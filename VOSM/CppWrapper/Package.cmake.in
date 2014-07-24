if(MSVC)
    set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
endif()
if(ENABLE_OPENMP)
    set(CMAKE_INSTALL_OPENMP_LIBRARIES 1)
endif()
#include(VOSMInstallRequiredSystemLibraries.cmake)
#include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "VOSM")
set(CPACK_PACKAGE_VENDOR "VOSM project Vision Open Statistical Models")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VOSM SDK for ${CMAKE_GENERATOR} is an vosm based open source for developing statistical models")

#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/doc/license.txt")

set(CPACK_PACKAGE_VERSION_MAJOR "${VOSM_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VOSM_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VOSM_VERSION_PATCH}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "vosm${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")

set(CPACK_PACKAGE_EXECUTABLES "")

set(CPACK_COMPONENTS_ALL main src Unspecified)

set(CPACK_COMPONENT_main_DISPLAY_NAME "Binaries and the Documentation")
set(CPACK_COMPONENT_src_DISPLAY_NAME "The source code")
#set(CPACK_COMPONENT_py_DISPLAY_NAME "Python Bindings")

set(CPACK_ALL_INSTALL_TYPES Full)

set(CPACK_COMPONENT_MAIN_INSTALL_TYPES Full)
set(CPACK_COMPONENT_SRC_INSTALL_TYPES Full)
#set(CPACK_COMPONENT_py_INSTALL_TYPES Full)

set(CPACK_SOURCE_IGNORE_FILES
    "/\\\\.svn/"
    "/autom4te.cache/"
    "/build/"
    "/lib/"
    "~$"    
    "\\\\.aux$"
    "\\\\.bbl$"
    "\\\\.blg$"
    "\\\\.idx$"
    "\\\\.ilg$"
    "\\\\.ind$"
    "\\\\.log$"
    "\\\\.toc$"
    "\\\\.out$"
    "\\\\.vcproj$"
    "/1$"
    "${CPACK_SOURCE_IGNORE_FILES}")

set(CPACK_SOURCE_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_SOURCE_GENERATOR "ZIP")
    set(CPACK_NSIS_PACKAGE_NAME "vosm ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}\\\\common\\\\vo.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}\\\\common\\\\vo.ico")
    #set(CPACK_PACKAGE_ICON "common/vo.ico") 
        
    set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_SOURCE_DIR}\\\\common\\\\vo.ico")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\visionopen.com")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\visionopen.com")
    set(CPACK_NSIS_CONTACT "jp4work@gmail.com")
    set(CPACK_NSIS_DISPLAY_NAME "vosm SDK")

    set(CPACK_NSIS_MENU_LINKS
        "doc\\\\index.htm" "Start Page"
        "doc\\\\vosm.pdf" "The Reference Manual"
        "CMakeLists.txt" "The Build Script (open with CMake)"
        "samples\\\\c" "C Samples"
        "samples\\\\python" "Python Samples")
    
    # Add "<install_path>/bin" to the system PATH
    set(CPACK_NSIS_MODIFY_PATH ON)
else()
    set(CPACK_GENERATOR "TBZ2")
    set(CPACK_SOURCE_GENERATOR "TBZ2")
    
    if(APPLE)
    set(CPACK_GENERATOR "PackageMaker;TBZ2")
    endif()
endif()

include(CPack)
