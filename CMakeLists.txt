cmake_minimum_required(VERSION 3.10)
project(potereides)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and build directories
set(SRC_DIR src)
set(BUILD_DIR build)

# Include all .cpp files in the src directory
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Set output directory for binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add executable
add_executable(potereides ${SOURCES})

# Link libraries
target_link_libraries(potereides m ncurses)

# Custom targets
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/potereides
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS potereides
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning all build files"
)
