#include <glad/glad.h>   // must come before any GL / GLFW header
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdio>

#include "gfx/Camera.h"
#include "gfx/Shader.h"

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

static Camera camera;

static void framebufferSizeCallback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
    if (h > 0) camera.aspect = (float)w / (float)h;
}

static void scrollCallback(GLFWwindow*, double, double yoffset) {
    camera.zoomBy((float)yoffset);
}

int main() {
    if (!glfwInit()) { std::printf("GLFW init failed\n"); return 1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "RubbleEngine", nullptr, nullptr);
    if (!window) { std::printf("Window creation failed\n"); glfwTerminate(); return 1; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::printf("GLAD init failed\n"); return 1;
    }

    std::printf("GPU: %s | GL %s\n",
                reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
                reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glEnable(GL_DEPTH_TEST);

    // Upload the cube.
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCube), kCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader(RUBBLE_SHADER_DIR "/basic.vert", RUBBLE_SHADER_DIR "/basic.frag");

    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    camera.aspect = fbh > 0 ? (float)fbw / (float)fbh : camera.aspect;

    // Edge-detect Q/E so one tap = one 90-degree snap.
    bool qWas = false, eWas = false;
    float last = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt  = now - last;
        last = now;

        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Pan with WASD.
        float fwd = 0.0f, right = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) fwd   -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) fwd   += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) right -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) right += 1.0f;
        if (fwd != 0.0f || right != 0.0f) camera.pan(fwd, right, dt);

        // Rotate 90 degrees with Q/E.
        bool q = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
        bool e = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
        if (q && !qWas) camera.rotateLeft();
        if (e && !eWas) camera.rotateRight();
        qWas = q; eWas = e;

        camera.update(dt);

        glClearColor(0.10f, 0.11f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("uProjection", camera.getProjection());
        shader.setMat4("uView",       camera.getView());
        shader.setMat4("uModel",      glm::mat4(1.0f));
        shader.setVec3("uColor",      glm::vec3(0.85f, 0.45f, 0.30f));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
