# Minimum 3.14 for FetchContent
cmake_minimum_required(VERSION 3.14)

# Set a traget for reuse
set(TARGET_NAME OpenGL_S01E01)
project(${TARGET_NAME})

# Prerequisites
include(FetchContent)
find_package(OpenGL REQUIRED)

# Fetch GLFW from GitHub
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.2
)

# Turn off some of the GLFW builds
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
FetchContent_MakeAvailable(glfw)

# Declare header file paths
include_directories(
    ${CMAKE_SOURCE_DIR}/3rdparty
    ${CMAKE_SOURCE_DIR}/3rdparty/glad/include
)

# Declare source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/3rdparty/glad/src/glad.c
)

# Configure build options
add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME}
    OpenGL::GL
    glfw
)

# Install the exceutable to specified location
install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
