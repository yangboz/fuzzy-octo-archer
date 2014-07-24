# ----------------------------------------------------------------------------
#  Root CMake file for vosm
#
#    From the off-tree build directory, invoke:
#      $ cmake <PATH_TO_vosm_ROOT>
#
#
#   - AUG-2011: Revised version <jp4work@gmail.com>
#
# ----------------------------------------------------------------------------

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
# Add these standard paths to the search paths for FIND_LIBRARY
# to find libraries from these locations first
if(UNIX)
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /lib /usr/lib)
endif()
# it _must_ go before PROJECT(vosm) in order to work
if (NOT CMAKE_INSTALL_PREFIX)
    if (WIN32)
        set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE INTERNAL "" FORCE)
    elseif()
        set(CMAKE_INSTALL_PREFIX "/usr" CACHE INTERNAL "" FORCE)
    endif()
endif()


set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_VERBOSE OFF CACHE BOOL "Verbose mode")

if (CMAKE_VERBOSE)
        set (CMAKE_VERBOSE_MAKEFILE 1)
endif()

project(vosm)

cmake_minimum_required(VERSION 2.4)

if(MSVC)
    set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

#set(CMAKE_C_COMPILER "/opt/BullseyeCoverage/bin/gcc")
#set(CMAKE_CXX_COMPILER "/opt/BullseyeCoverage/bin/g++")
#set(CMAKE_CXX_COMPILER_INIT "/opt/BullseyeCoverage/bin/gcc")

# --------------------------------------------------------------
# Indicate CMake 2.7 and above that we don't want to mix relative
#  and absolute paths in linker lib lists.
# Run "cmake --help-policy CMP0003" for more information.
# --------------------------------------------------------------
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif()

# ----------------------------------------------------------------------------
#  Current version number:
# ----------------------------------------------------------------------------
set(VOSM_VERSION "0.3.2")

string(REGEX MATCHALL "[0-9]" VOSM_VERSION_PARTS "${VOSM_VERSION}")

list(GET VOSM_VERSION_PARTS 0 VOSM_VERSION_MAJOR)
list(GET VOSM_VERSION_PARTS 1 VOSM_VERSION_MINOR)
list(GET VOSM_VERSION_PARTS 2 VOSM_VERSION_PATCH)

set(VOSM_SOVERSION "${VOSM_VERSION_MAJOR}.${VOSM_VERSION_MINOR}")

if(WIN32)
    # Postfix of DLLs:
    set(VOSM_DLLVERSION "${VOSM_VERSION_MAJOR}${VOSM_VERSION_MINOR}${VOSM_VERSION_PATCH}")
    set(VOSM_DEBUG_POSTFIX d)
else()
    # Postfix of so's:
    #set(VOSM_DLLVERSION "${VOSM_VERSION_MAJOR}${VOSM_VERSION_MINOR}${VOSM_VERSION_PATCH}")
    set(VOSM_DLLVERSION "")
    set(VOSM_DEBUG_POSTFIX)
endif()


# ----------------------------------------------------------------------------
# Build static or dynamic libs?
# ----------------------------------------------------------------------------
# Default: dynamic libraries:
SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)")
IF(BUILD_SHARED_LIBS)
    SET(VOSM_BUILD_SHARED_LIB 1) # For cvconfig.h, etc.
ELSE(BUILD_SHARED_LIBS)
    SET(VOSM_BUILD_SHARED_LIB 0)
ENDIF(BUILD_SHARED_LIBS)

# ----------------------------------------------------------------------------
#  Variables for cvconfig.h.cmake
# ----------------------------------------------------------------------------
set(PACKAGE "vosm")
set(PACKAGE_BUGREPORT "jp4work@gmail.com")
set(PACKAGE_NAME "vosm")
set(PACKAGE_STRING "${PACKAGE} ${VOSM_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_VERSION "${VOSM_VERSION}")

# ----------------------------------------------------------------------------
#  Autodetect if we are in a SVN repository
# ----------------------------------------------------------------------------
find_program(SVNVERSION_PATH svnversion)
mark_as_advanced(force SVNVERSION_PATH)
if(SVNVERSION_PATH)
    message(STATUS "Extracting svn version, please wait...")
    execute_process(COMMAND ${SVNVERSION_PATH} -n ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE SVNVERSION_RESULT)

    if(SVNVERSION_RESULT MATCHES "exported")
       # This is NOT a svn repository:
       set(VOSM_SVNVERSION "")
    else()
        set(VOSM_SVNVERSION " svn:${SVNVERSION_RESULT}")
    endif()

    message(STATUS "SVNVERSION: ${VOSM_SVNVERSION}")
else()
    # We don't have svnversion:
    set(VOSM_SVNVERSION "")
endif()

# ----------------------------------------------------------------------------
# Detect GNU version:
# ----------------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version
                  OUTPUT_VARIABLE CMAKE_VOSM_GCC_VERSION_FULL
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Typical output in CMAKE_VOSM_GCC_VERSION_FULL: "c+//0 (whatever) 4.4.5 (...)"
    #  Look for the version number
    string(REGEX MATCH "[0-9].[0-9].[0-9]" CMAKE_GCC_REGEX_VERSION "${CMAKE_VOSM_GCC_VERSION_FULL}")

    # Split the three parts:
    string(REGEX MATCHALL "[0-9]" CMAKE_VOSM_GCC_VERSIONS "${CMAKE_GCC_REGEX_VERSION}")

    list(GET CMAKE_VOSM_GCC_VERSIONS 0 CMAKE_VOSM_GCC_VERSION_MAJOR)
    list(GET CMAKE_VOSM_GCC_VERSIONS 1 CMAKE_VOSM_GCC_VERSION_MINOR)

    set(CMAKE_VOSM_GCC_VERSION ${CMAKE_VOSM_GCC_VERSION_MAJOR}${CMAKE_VOSM_GCC_VERSION_MINOR})
    math(EXPR CMAKE_VOSM_GCC_VERSION_NUM "${CMAKE_VOSM_GCC_VERSION_MAJOR}*100 + ${CMAKE_VOSM_GCC_VERSION_MINOR}")
    message(STATUS "Detected version of GNU GCC: ${CMAKE_VOSM_GCC_VERSION} (${CMAKE_VOSM_GCC_VERSION_NUM})")

    if(WIN32)
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
                  OUTPUT_VARIABLE CMAKE_VOSM_GCC_TARGET_MACHINE
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(CMAKE_VOSM_GCC_TARGET_MACHINE MATCHES "64")
            set(MINGW64 1)
        endif()
    endif()

endif()

# ----------------------------------------------------------------------------
#       CHECK FOR SYSTEM LIBRARIES, OPTIONS, ETC..
# ----------------------------------------------------------------------------

# Build/install (or not) some apps:
# ===================================================
set(BUILD_EXAMPLES ON CACHE BOOL "Build all examples")
set(INSTALL_C_EXAMPLES ON CACHE BOOL "Install C examples")
#set(INSTALL_PYTHON_EXAMPLES OFF CACHE BOOL "Install Python examples")

# Build tests:
# ===================================================
set(BUILD_TESTS ON CACHE BOOL "Build tests")

include(vosmModule.cmake REQUIRED)

if(UNIX)
    include(VOSMFindPkgConfig.cmake OPTIONAL)
    include(CheckFunctionExists)
    include(CheckIncludeFile)
endif()

# allow fine grained control over which libraries not to link, even if
# they are available on the system
# ====================================================================
set(WITH_BOOST_REGEX ON CACHE BOOL "Include BOOST REGEX support")
set(WITH_BOOST_FILESYSTEM ON CACHE BOOL "Include BOOST FILESYSTEM support")
set(WITH_BOOST_SYSTEM ON CACHE BOOL "Include BOOST SYSTEM support")
set(WITH_OPENCV ON CACHE BOOL "Include OPENCV 2.x support")

# ===================================================
# Macros that checks if module have been installed.
# After it adds module to build and define
# constants passed as second arg
# ===================================================

macro(CHECK_MODULE module_name define)
    set(${define} 0)
    if(PKG_CONFIG_FOUND)
        set(ALIAS               ALIASOF_${module_name})
        set(ALIAS_FOUND                 ${ALIAS}_FOUND)
        set(ALIAS_INCLUDE_DIRS   ${ALIAS}_INCLUDE_DIRS)
        set(ALIAS_LIBRARY_DIRS   ${ALIAS}_LIBRARY_DIRS)
        set(ALIAS_LIBRARIES         ${ALIAS}_LIBRARIES)

        PKG_CHECK_MODULES(${ALIAS} ${module_name})
        if (${ALIAS_FOUND})
            set(${define} 1)
			foreach(P "${ALIAS_INCLUDE_DIRS}")
                if (${P})
                    list(APPEND HIGHGUI_INCLUDE_DIRS ${${P}})
                endif()
            endforeach()

            foreach(P "${ALIAS_LIBRARY_DIRS}")
                if (${P})
                    list(APPEND HIGHGUI_LIBRARY_DIRS ${${P}})
                endif()
            endforeach()

			list(APPEND HIGHGUI_LIBRARIES ${${ALIAS_LIBRARIES}})
		endif()
    endif()
endmacro()

set(VOSM_LINKER_LIBS ${VOSM_LINKER_LIBS} ${OPENCV_LINKER_LIBS} )


if(WITH_BOOST_REGEX)
  CHECK_MODULE(libboost-regex HAVE_BOOST_REGEX)
else()
  set(HAVE_BOOST_REGEX FALSE)
endif()

if(WITH_BOOST_FILESYSTEM)
  CHECK_MODULE(libboost-filesystem HAVE_BOOST_FILESYSTEM)
else()
  set(HAVE_BOOST_FILESYSTEM FALSE)
endif()

if(WITH_OPENCV)
    find_path(OPENCV_INCLUDE_PATH "cv.h"
        PATHS "/usr/include/opencv" "/usr/local/include/opencv"
        DOC "The path to OPENCV headers")
    if(OPENCV_INCLUDE_PATH)
        include_directories(${OPENCV_INCLUDE_PATH})
        set(HAVE_OPENCV TRUE)
    endif()
    CHECK_MODULE(libopencv_calib3d HAVE_OPENCV_CALIB3D)
    CHECK_MODULE(libopencv_contrib HAVE_OPENCV_CONTRIB)
    CHECK_MODULE(libopencv_core HAVE_OPENCV_CORE)
    CHECK_MODULE(libopencv_features2d HAVE_OPENCV_FEATURE2D)
    CHECK_MODULE(libopencv_gpu HAVE_OPENCV_GPU)
    CHECK_MODULE(libopencv_highgui HAVE_OPENCV_HIGHGUI)
    CHECK_MODULE(libopencv_imgproc HAVE_OPENCV_IMGPROC)
    CHECK_MODULE(libopencv_legacy HAVE_OPENCV_LEGACY)
    CHECK_MODULE(libopencv_ml HAVE_OPENCV_ML)
    CHECK_MODULE(libopencv_objdetect HAVE_OPENCV_OBJDETECT)
    CHECK_MODULE(libopencv_video HAVE_OPENCV_VIDEO)
  else()
    set(HAVE_OPENCV FALSE)
endif()


# ----------------------------------------------------------------------------
#                   UPDATE CONFIG FILES & SCRIPTS:
#
#  CONFIGURE_FILE(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
# If @ONLY is specified, only variables of the form @VAR@ will be
#  replaces and ${VAR} will be ignored.
#
#  A directory will be created for each platform so the "cvconfig.h" file is
#   not overwritten if cmake generates code in the same path.
# ----------------------------------------------------------------------------
add_definitions(-DHAVE_CONFIG_H)

set(VOSM_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant cvconfig.h")

message(STATUS "Parsing 'cvconfig.h.cmake'")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cvconfig.h.cmake" "${VOSM_CONFIG_FILE_INCLUDE_DIR}/cvconfig.h")

# ---------------------------------------------------------------------------
# The C+//0 include & link directories:
# ---------------------------------------------------------------------------
include_directories("."
    "${OPENCV_INCLUDE_PATH}"
    "${VOSM_CONFIG_FILE_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/opencv"
    )

# ----------------------------------------------------------------------------
#           Set the maximum level of warnings:
# ----------------------------------------------------------------------------
# Should be set to true for development
set(VOSM_WARNINGS_ARE_ERRORS OFF CACHE BOOL "Treat warnings as errors")

set(EXTRA_C_FLAGS "")
set(EXTRA_C_FLAGS_RELEASE "")
set(EXTRA_C_FLAGS_DEBUG "")
set(EXTRA_EXE_LINKER_FLAGS "")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "")

if(MSVC)
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _SCL_SECURE_NO_WARNINGS")
    # 64-bit portability warnings, in MSVC8
    if(MSVC80)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Wp64")
    endif()
    #if(MSVC90)
    #    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /D _BIND_TO_CURRENT_CRT_VERSION=1 /D _BIND_TO_CURRENT_VCLIBS_VERSION=1")
    #endif()

    set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /debug")

    #if(ENABLE_OPENMP)
    #    set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /openmp")
    #endif()

    # Remove unreferenced functions: function level linking
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Gy")
    set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Zi")
endif()

# ----------------------------------------------------------------------------
#                       PROCESS SUBDIRECTORIES:
# ----------------------------------------------------------------------------
# Save libs and executables in the same place
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH "Output directory for libraries" )
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH "Output directory for applications" )

if(WIN32)
set(VOSM_DOC_INSTALL_PATH doc)
else()
set(VOSM_DOC_INSTALL_PATH share/vosm/doc)
endif()

# --------------------------------------------------------------------------------------------
#   Installation for CMake Module:  vosmConfig.cmake
#  Part 1/3: ${BIN_DIR}/vosmConfig.cmake              -> For use *without* "make install"
#  Part 2/3: ${BIN_DIR}/unix-install/vosmConfig.cmake -> For use with "make install"
#  Part 3/3: ${BIN_DIR}/win-install/vosmConfig.cmake  -> For use within binary installers/packages
# -------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------------------------
#  Part 1/3: ${BIN_DIR}/vosmConfig.cmake              -> For use *without* "make install"
# -------------------------------------------------------------------------------------------

# Name of libraries is: libcv.so.1.1.0, etc...
#  VOSM_DLLVERSION must be set to "110", etc..
# Already done above.

# Set CMAKE_INCLUDE_DIRS_CONFIGCMAKE to the list of include directories:
set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE "\"${VOSM_CONFIG_FILE_INCLUDE_DIR}\" \"${CMAKE_CURRENT_SOURCE_DIR}/include\" \"${CMAKE_CURRENT_SOURCE_DIR}/include/vosm\"")
set(CMAKE_BASE_INCLUDE_DIRS_CONFIGCMAKE "\"${CMAKE_CURRENT_SOURCE_DIR}\"")

set(CMAKE_LIB_DIRS_CONFIGCMAKE "${LIBRARY_OUTPUT_PATH}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vosmConfig.cmake.in" "${CMAKE_BINARY_DIR}/vosmConfig.cmake" IMMEDIATE @ONLY)

# --------------------------------------------------------------------------------------------
#  Part 2/3: ${BIN_DIR}/unix-install/vosmConfig.cmake -> For use *with* "make install"
# -------------------------------------------------------------------------------------------
# Set CMAKE_INCLUDE_DIRS_CONFIGCMAKE to the list of include directories:

set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/include/opencv" "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_BASE_INCLUDE_DIRS_CONFIGCMAKE "\"\"")

set(CMAKE_LIB_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/lib")

#exec_program(${CMAKE_COMMAND} ARGS "-E make_directory \"${CMAKE_BINARY_DIR}/unix-install/\"")
exec_program(mkdir ARGS "-p \"${CMAKE_BINARY_DIR}/unix-install/\"")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vosmConfig.cmake.in" "${CMAKE_BINARY_DIR}/unix-install/vosmConfig.cmake" IMMEDIATE @ONLY)

if(UNIX)
    # For a command "FIND_PACKAGE(FOO)", CMake will look at the directory /usr/share|lib/FOO/FOOConfig.cmake, so:
    install(FILES ${CMAKE_BINARY_DIR}/unix-install/vosmConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/share/vosm/)
endif()

# --------------------------------------------------------------------------------------------
#  Part 3/3: ${BIN_DIR}/win-install/vosmConfig.cmake  -> For use within binary installers/packages
# -------------------------------------------------------------------------------------------
if(WIN32)
    # Set CMAKE_INCLUDE_DIRS_CONFIGCMAKE to the list of include directories:

    # This will expand to, for example, <program files>/vosm 0.3.2/include
    set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE "\"\${THIS_VOSM_CONFIG_PATH}/include\" \"\${THIS_VOSM_CONFIG_PATH}/include/vosm\"")
        set(CMAKE_BASE_INCLUDE_DIRS_CONFIGCMAKE "\"${THIS_VOSM_CONFIG_PATH}\"")

    # This will expand to, for example, <program files>/vosm 0.3.2/lib
    set(CMAKE_LIB_DIRS_CONFIGCMAKE "\"\${THIS_VOSM_CONFIG_PATH}/lib\"")

    #exec_program(${CMAKE_COMMAND} ARGS "-E make_directory \"${CMAKE_BINARY_DIR}/win-install/\"")
    exec_program(mkdir ARGS "-p \"${CMAKE_BINARY_DIR}/win-install/\"")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vosmConfig.cmake.in" "${CMAKE_BINARY_DIR}/win-install/vosmConfig.cmake" IMMEDIATE @ONLY)
endif()


# --------------------------------------------------------------------------------------------
#  Part 2/2: ${BIN_DIR}/unix-install/vosm.pc -> For use *with* "make install"
# prefix=/usr
# exec_prefix=${prefix}
# libdir=${exec_prefix}/lib
# includedir=${prefix}/include/vosm
# -------------------------------------------------------------------------------------------
set(opencvdir      ${OPENCV_INCLUDE_PATH})
set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix     "\${prefix}")
set(libdir      "\${exec_prefix}/lib")
set(includedir      "\${prefix}/include")
set(VERSION             ${VOSM_VERSION})


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/vosm.pc.cmake.in" "${CMAKE_BINARY_DIR}/unix-install/vosm.pc" @ONLY IMMEDIATE)

if(UNIX)
    install(FILES ${CMAKE_BINARY_DIR}/unix-install/vosm.pc DESTINATION lib/pkgconfig)
endif()


# ----------------------------------------------------------------------------
#   Uninstall target, for "make uninstall"
# ----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")


# ----------------------------------------------------------------------------
#   CPack target
# ----------------------------------------------------------------------------

set(BUILD_PACKAGE ON CACHE BOOL "Build a installer with the SDK")
message(STATUS "BUILD_PACKAGE FALSE")
if(BUILD_PACKAGE)
message(STATUS "BUILD_PACKAGE TRUE")
    configure_file(
    Package.cmake.in
    ${CMAKE_BINARY_DIR}/.cpack/Package.cmake
    @ONLY
    )

    include(${CMAKE_BINARY_DIR}/.cpack/Package.cmake)

    set(root_files README)

    file(GLOB root_src_files *.in *.cmake CMakeLists.txt)

    if (NOT WIN32)
        install(FILES ${root_files}
                DESTINATION ${VOSM_DOC_INSTALL_PATH}
                COMPONENT main)
    else()
        install(FILES ${root_files}
                DESTINATION "."
                COMPONENT main)
        install(FILES ${root_src_files}
                DESTINATION "."
                COMPONENT src)
        # Add the "win-install/OpenCVConfig.cmake" file to the "main" install component
        install(FILES
            "${CMAKE_BINARY_DIR}/win-install/OpenCVConfig.cmake"
            DESTINATION "."
            COMPONENT main
            )

        if(CMAKE_INSTALL_DEBUG_LIBRARIES)
            foreach(m cxcore cv cvaux ml highgui opencv_ffmpeg)
                install(FILES ${CMAKE_BINARY_DIR}/bin/Debug/${m}${VOSM_DLLVERSION}d.dll
            DESTINATION bin COMPONENT main)
                install(FILES ${CMAKE_BINARY_DIR}/lib/Debug/${m}${VOSM_DLLVERSION}d.lib
            DESTINATION lib COMPONENT main)
            endforeach()
        endif()

		install(FILES AUTHORS CHANGES COPYRIGHT HOWTOUSE INSTALL README TODO VERSION DESTINATION share/doc/vosm)
        install(DIRECTORY doc/ DESTINATION doc COMPONENT src FILES_MATCHING PATTERN "*.tex")

    endif()
endif()


#-----------------------------------
# Subdirectories:
#-----------------------------------
add_subdirectory(modules)


if(BUILD_LATEX_DOCS)
  include(OpenCVFindLATEX.cmake REQUIRED)

  if(PDFLATEX_COMPILER)
    message(STATUS "PDF LaTeX found!")
  endif()
endif()


#if(BUILD_EXAMPLES OR INSTALL_PYTHON_EXAMPLES)
if(BUILD_EXAMPLES)
    add_subdirectory(tests)
endif()


#----------------------------------------------------------------------------
#   Generate the vosmModule.cmake file for unix
#      installation in CMAKE_INSTALL_PREFIX
#----------------------------------------------------------------------------
