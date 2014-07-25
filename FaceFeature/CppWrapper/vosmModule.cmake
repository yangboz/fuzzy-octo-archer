# this is template for a VOSM module 
macro(define_vosm_module name)
	
	project(${name})
	add_definitions(-DCVAPI_EXPORTS)

	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
	                    "${CMAKE_CURRENT_SOURCE_DIR}/src"
	                    "${CMAKE_CURRENT_BINARY_DIR}")

	file(GLOB lib_srcs "src/*.cpp")
	file(GLOB lib_hdrs "include/*.h*")
	source_group("Src" FILES ${lib_srcs} ${lib_hdrs})

	set(the_target "${name}")

	add_library(${the_target} ${lib_srcs} ${lib_hdrs} )


	# For dynamic link numbering convenions
	set_target_properties(${the_target} PROPERTIES
	    VERSION ${VOSM_VERSION}
	    SOVERSION ${VOSM_SOVERSION}
	    OUTPUT_NAME "${the_target}${VOSM_DLLVERSION}"
	    )

	# Additional target properties
	set_target_properties(${the_target} PROPERTIES
	    DEBUG_POSTFIX "${VOSM_DEBUG_POSTFIX}"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
	    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	    )

	# Add the required libraries for linking:
	target_link_libraries(${the_target} ${VOSM_LINKER_LIBS} ${IPP_LIBS} )

	if(MSVC)
	    if(CMAKE_CROSSCOMPILING)
	        set_target_properties(${the_target} PROPERTIES
	            LINK_FLAGS "/NODEFAULTLIB:secchk"
	            )
	    endif()
	    set_target_properties(${the_target} PROPERTIES
	        LINK_FLAGS "/NODEFAULTLIB:libc"
	        )
	endif()

	# Dependencies of this target:
	add_dependencies(${the_target} ${ARGN})

	install(TARGETS ${the_target}
	    RUNTIME DESTINATION bin COMPONENT main
	    LIBRARY DESTINATION lib COMPONENT main
	    ARCHIVE DESTINATION lib COMPONENT main)

	install(FILES ${lib_hdrs}
	    DESTINATION include/vosm/${name}
	    COMPONENT main)

endmacro()
