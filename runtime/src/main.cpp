#include <glad/glad.h>   // must come before any GL / GLFW header
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <string>

#include "gfx/Camera.h"
#include "gfx/Shader.h"
#include "project/Project.h"
#include "Window.h"

// Unit cube spanning [-1, 1], 36 vertices, each: position (vec3) + normal (vec3).
static const float kCube[] = {
    // back face (-Z)
    -1,-1,-1,  0, 0,-1,   1, 1,-1,  0, 0,-1,   1,-1,-1,  0, 0,-1,
     1, 1,-1,  0, 0,-1,  -1,-1,-1,  0, 0,-1,  -1, 1,-1,  0, 0,-1,
    // front face (+Z)
    -1,-1, 1,  0, 0, 1,   1,-1, 1,  0, 0, 1,   1, 1, 1,  0, 0, 1,
     1, 1, 1,  0, 0, 1,  -1, 1, 1,  0, 0, 1,  -1,-1, 1,  0, 0, 1,
    // left face (-X)
    -1, 1, 1, -1, 0, 0,  -1, 1,-1, -1, 0, 0,  -1,-1,-1, -1, 0, 0,
    -1,-1,-1, -1, 0, 0,  -1,-1, 1, -1, 0, 0,  -1, 1, 1, -1, 0, 0,
    // right face (+X)
     1, 1, 1,  1, 0, 0,   1,-1,-1,  1, 0, 0,   1, 1,-1,  1, 0, 0,
     1,-1,-1,  1, 0, 0,   1, 1, 1,  1, 0, 0,   1,-1, 1,  1, 0, 0,
    // bottom face (-Y)
    -1,-1,-1,  0,-1, 0,   1,-1,-1,  0,-1, 0,   1,-1, 1,  0,-1, 0,
     1,-1, 1,  0,-1, 0,  -1,-1, 1,  0,-1, 0,  -1,-1,-1,  0,-1, 0,
    // top face (+Y)
    -1, 1,-1,  0, 1, 0,   1, 1, 1,  0, 1, 0,   1, 1,-1,  0, 1, 0,
     1, 1, 1,  0, 1, 0,  -1, 1,-1,  0, 1, 0,  -1, 1, 1,  0, 1, 0,
};

int main(int argc, char** argv) {
    // Parse --project <path> (also -p).
    std::string projectPath;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--project" || arg == "-p") && i + 1 < argc)
            projectPath = argv[++i];
    }

    // Load a level from the project, if one was given.
    Level level;
    if (!projectPath.empty()) {
        Project project;
        if (project.open(projectPath) && project.loadStartLevel(level))
            std::printf("Opened project '%s' (%zu objects)\n",
                        project.name.c_str(), level.objects.size());
        else
            std::printf("Failed to open project at %s\n", projectPath.c_str());
    }
    // Fall back to a single cube so the app always shows something.
    if (level.objects.empty())
        level.objects.push_back({ "cube", glm::vec3(0.0f) });

    Window window("RubbleEngine", 1280, 720);
    Camera camera;

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao); glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCube), kCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader(RUBBLE_SHADER_DIR "/basic.vert", RUBBLE_SHADER_DIR "/basic.frag");

    bool qWas = false, eWas = false;
    while (window.isOpen()) {
        window.beginFrame();
        float dt = window.deltaTime();

        if (window.key(GLFW_KEY_ESCAPE)) window.close();
        float fwd = 0, right = 0;
        if (window.key(GLFW_KEY_W)) fwd -= 1;
        if (window.key(GLFW_KEY_S)) fwd += 1;
        if (window.key(GLFW_KEY_A)) right -= 1;
        if (window.key(GLFW_KEY_D)) right += 1;
        if (fwd || right) camera.pan(fwd, right, dt);

        bool q = window.key(GLFW_KEY_Q), e = window.key(GLFW_KEY_E);
        if (q && !qWas) camera.rotateLeft();
        if (e && !eWas) camera.rotateRight();
        qWas = q; eWas = e;

        camera.zoomBy(window.scrollDelta());
        camera.aspect = window.aspect();
        camera.update(dt);

        shader.use();
        shader.setMat4("uProjection", camera.getProjection());
        shader.setMat4("uView", camera.getView());
        shader.setVec3("uColor", glm::vec3(0.85f, 0.45f, 0.30f));
        glBindVertexArray(vao);
        for (const LevelObject& obj : level.objects) {
            // The GAME decides what each type means. This runtime only knows "cube".
            if (obj.type == "cube") {
                shader.setMat4("uModel", glm::translate(glm::mat4(1.0f), obj.position));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        window.endFrame();
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    return 0;
}
