
# Generate meta-header
configure_file(glhimmel-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/glhimmel-version.h)

# We do NOT want to deploy this header (at least not for binary packages)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/glhimmel/glhimmel-version.h DESTINATION ${INSTALL_INCLUDE}/glhimmel)

# Include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Libraries
set(IDE_FOLDER "")
add_subdirectory(glhimmel-texturebased)

# Applications
set(IDE_FOLDER "Applications")
add_subdirectory(examples)
