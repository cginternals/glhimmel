
# Check if examples are enabled
if(NOT OPTION_BUILD_EXAMPLES)
    return()
endif()

# Example applications
add_subdirectory(debugger)
add_subdirectory(demo_textured)
add_subdirectory(demo_computed)
