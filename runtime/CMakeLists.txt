# runtime — a minimal GLFW app that drives EngineCore. This is where the window
# and OpenGL context are created; the engine itself stays windowing-agnostic.
add_executable(runtime src/main.cpp)

target_link_libraries(runtime PRIVATE EngineCore glfw)
if(WIN32)
    target_link_libraries(runtime PRIVATE opengl32)
endif()

# Absolute path to the shaders folder so the app finds them no matter where it
# is launched from.
target_compile_definitions(runtime PRIVATE
    RUBBLE_SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders")
