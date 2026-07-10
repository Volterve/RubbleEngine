cmake_minimum_required(VERSION 3.20)
project(RubbleEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Some fetched dependencies (e.g. glad 0.1.x) still declare compatibility with
# very old CMake, which CMake 4.x rejects. This lets them configure anyway.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# ── Dependencies (downloaded automatically on first configure) ────────────────
# Needs an internet connection the first time you run CMake, plus Python 3 on
# PATH (glad uses it to generate the GL loader).
include(FetchContent)

# glad — OpenGL 3.3 core function loader
set(GLAD_PROFILE   "core"   CACHE STRING "" FORCE)
set(GLAD_API       "gl=3.3" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c"      CACHE STRING "" FORCE)
FetchContent_Declare(glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.36
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(glad)

# GLFW — window, input, and OpenGL context creation
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.10
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(glfw)

# glm — header-only math library
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(glm)

add_subdirectory(engine)
add_subdirectory(runtime)
