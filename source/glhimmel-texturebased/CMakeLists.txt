
set(target glhimmel-texturebased)
message(STATUS "Lib ${target}")


# External libraries

find_package(Qt5Core    5.1 REQUIRED)
find_package(Qt5Gui     5.1 REQUIRED)
find_package(Qt5Widgets 5.1 REQUIRED) # this is also important in order to get uic working 


# Includes

include_directories(
    # e.g., ${TRDP_INCLUDE_DIR}
)

include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Libraries

set(libs
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)


# Compiler definitions

if (OPTION_BUILD_STATIC)
    add_definitions("-D${META_PROJECT_NAME_UPPER}_STATIC")
else()
    add_definitions("-DFIBLIB_EXPORTS")
endif()


# Sources

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(api_includes
    ${include_path}/glhimmel-texturebased_api.h
    ${include_path}/abstracthimmel.h
    ${include_path}/abstractmappedhimmel.h
    ${include_path}/atime.h
    ${include_path}/coords.h
    ${include_path}/cubemappedhimmel.h
    ${include_path}/himmelenvmap.h
    ${include_path}/himmelquad.h
    ${include_path}/horizonband.h
    ${include_path}/interpolate.h
    ${include_path}/julianday.h
    ${include_path}/mathmacros.h
    ${include_path}/osgposter.h
    ${include_path}/paraboloidmappedhimmel.h
    ${include_path}/polarmappedhimmel.h
    ${include_path}/pragmanote.h
    ${include_path}/spheremappedhimmel.h
    ${include_path}/timef.h
    ${include_path}/twounitschanger.h
    ${include_path}/typedefs.h
)

set(sources
    ${source_path}/abstracthimmel.cpp
    ${source_path}/abstractmappedhimmel.cpp
    ${source_path}/atime.cpp
    ${source_path}/coords.cpp
    ${source_path}/cubemappedhimmel.cpp
    ${source_path}/himmelenvmap.cpp
    ${source_path}/himmelquad.cpp
    ${source_path}/horizonband.cpp
    ${source_path}/julianday.cpp
    ${source_path}/osgposter.cpp
    ${source_path}/paraboloidmappedhimmel.cpp
    ${source_path}/polarmappedhimmel.cpp
    ${source_path}/spheremappedhimmel.cpp
    ${source_path}/timef.cpp
    ${source_path}/twounitschanger.cpp
)

# Group source files
set(header_group "Header Files (API)")
set(source_group "Source Files")
source_group_by_path(${include_path} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${api_includes})
source_group_by_path(${source_path} "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# Build library

add_library(${target} ${api_includes} ${sources})

target_link_libraries(${target} ${libs})

target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_FLAGS})

set_target_properties(${target}
    PROPERTIES
    LINKER_LANGUAGE              CXX
    FOLDER                      "${IDE_FOLDER}"
    COMPILE_DEFINITIONS_DEBUG   "${DEFAULT_COMPILE_DEFS_DEBUG}"
    COMPILE_DEFINITIONS_RELEASE "${DEFAULT_COMPILE_DEFS_RELEASE}"
    LINK_FLAGS_DEBUG            "${DEFAULT_LINKER_FLAGS_DEBUG}"
    LINK_FLAGS_RELEASE          "${DEFAULT_LINKER_FLAGS_RELEASE}"
    DEBUG_POSTFIX               "d${DEBUG_POSTFIX}"
    INCLUDE_PATH	            ${include_path})


# Deployment

# Library
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN}
    LIBRARY DESTINATION ${INSTALL_SHARED}
    ARCHIVE DESTINATION ${INSTALL_LIB}
)

# Header files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE})
