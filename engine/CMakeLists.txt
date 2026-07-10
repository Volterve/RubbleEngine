# EngineCore — the reusable engine, built as a static library.
#
# It deliberately links glad (a GL *loader*) and glm (math) but NOT glfw: the
# engine must not know how its OpenGL context was created. That keeps it usable
# from the GLFW runtime today and from a Qt-based level editor later.
add_library(EngineCore STATIC
    src/gfx/Shader.cpp
)

target_include_directories(EngineCore PUBLIC src)
target_link_libraries(EngineCore PUBLIC glad glm::glm)
