cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(WowMapView)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#if (MSVC)
#	set(CMAKE_GENERATOR_PLATFORM Win32)
#endif()

# Compiler options
if(MSVC)
    # Define preprocessor definitions and MSVC-specific options
    add_compile_definitions(WIN32 _WINDOWS)
    # For MSVC, disable warnings about the unchecked std::copy
    add_compile_options(/EHsc)
	
    # Debug-specific options
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    # Release-specific options
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
else()
    add_compile_options(-D_LINUX -g)
endif()

# Find packages
if (NOT MSVC)
	find_package(SDL REQUIRED)
	find_package(OpenGL REQUIRED)
endif()

# Include directories
if(MSVC)
	include_directories(
		${PROJECT_SOURCE_DIR}
		${PROJECT_SOURCE_DIR}/zlib
		${PROJECT_SOURCE_DIR}/SDL/include
		${PROJECT_SOURCE_DIR}/bzip2
		#stormlib
		libmpq
		${OPENGL_INCLUDE_DIR}
	)
else()
	include_directories(. stormlib zlib libmpq ${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
endif()

# Add the stormlib directory as a subdirectory so its CMakeLists.txt is processed
add_subdirectory(stormlib)

# Source files
set(SOURCES
    areadb.cpp
    dbcfile.cpp
    font.cpp
    frustum.cpp
    liquid.cpp
    particle.cpp
    maptile.cpp
    menu.cpp
    model.cpp
    mpq_stormlib.cpp
    shaders.cpp
    sky.cpp
    test.cpp
    video.cpp
    wmo.cpp
    world.cpp
    wowmapview.cpp
    util.cpp
)

# Define the executable
add_executable(wowmapview ${SOURCES})

# Ensure stormlib is built before wowmapview
add_dependencies(wowmapview stormlib)

# Specify libraries to link against
if (MSVC)
	#target_compile_options(wowmapview PRIVATE
    #    $<$<CONFIG:Debug>:/MDd> # Use /MDd for Debug builds
    #    $<$<CONFIG:Release>:/MD> # Use /MD for Release builds
    #)
    
    # If there are specific linker flags you need to adjust for debug or release, use target_link_options in a similar manner:
    #target_link_options(wowmapview PRIVATE
    #    $<$<CONFIG:Debug>:/SOME_DEBUG_OPTION>
    #    $<$<CONFIG:Release>:/SOME_RELEASE_OPTION>
    #)
	
	# Do not use... Even if it seems to be in original vcxproj?
	#target_link_options(wowmapview PRIVATE
    #    "/NODEFAULTLIB:LIBCMT.lib"
    #)
	
	target_link_options(wowmapview PRIVATE
        "/MACHINE:X86"
    )

	target_link_libraries(wowmapview
		OpenGL32.lib
		GLu32.lib
		#SDL2.lib # SDL 2.x
		#SDL2main.lib # SDL 2.x
		SDL.lib
		SDLmain.lib
		#stormlib
		#legacy_stdio_definitions.lib
	)
	
else()
	target_link_libraries(wowmapview
		${SDL_LIBRARY}
		${OPENGL_LIBRARIES}
		bz2
		stormlib
	)
endif()

if(MSVC)
    # Additional library directories for MSVC
	target_link_directories(wowmapview PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/Windows/32bit
        ${PROJECT_SOURCE_DIR}/SDL/lib
		$<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/stormlib/Debug>
		$<$<CONFIG:Release>:${CMAKE_BINARY_DIR}/stormlib/Release>
    )
	
    # Copying DLLs and other necessary files
    add_custom_command(TARGET wowmapview POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../bin/arial.tga" $<TARGET_FILE_DIR:wowmapview>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../bin/arial.info" $<TARGET_FILE_DIR:wowmapview>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../bin/SDL.dll" $<TARGET_FILE_DIR:wowmapview>
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/../bin/shaders/" $<TARGET_FILE_DIR:wowmapview>/shaders
    )
else()
    # Post-build copy command for non-MSVC environments
    add_custom_command(TARGET wowmapview POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:wowmapview> ${PROJECT_SOURCE_DIR}/../bin/
    )
endif()
