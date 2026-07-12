#include "Window.h"
#include <cstdio>

Window::Window(const std::string& title, int width, int height)
    : width_(width), height_(height) {
    if (!glfwInit()) { std::printf("GLFW init failed\n"); return; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) { std::printf("Window creation failed\n"); glfwTerminate(); return; }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window_, this); // so callbacks find us
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    glfwSetScrollCallback(window_, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::printf("GLAD init failed\n"); return;
    }
    glfwGetFramebufferSize(window_, &width_, &height_);
    glViewport(0, 0, width_, height_);
    glEnable(GL_DEPTH_TEST);
    last_ = (float)glfwGetTime();
}

Window::~Window() {
    if (window_) glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Window::isOpen() const { return window_ && !glfwWindowShouldClose(window_); }

void Window::beginFrame() {
    float now = (float)glfwGetTime();
    dt_ = now - last_;
    last_ = now;

    scroll_ = 0.0f;
    glfwPollEvents();

    glClearColor(clearColor_[0], clearColor_[1], clearColor_[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::endFrame() { glfwSwapBuffers(window_); }
void Window::close() { glfwSetWindowShouldClose(window_, true); }

bool  Window::key(int glfwKey) const { return glfwGetKey(window_, glfwKey) == GLFW_PRESS; }
float Window::aspect() const { return height_ > 0 ? (float)width_ / (float)height_ : 1.0f; }

void Window::framebufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (self) { self->width_ = width; self->height_ = height; }
}

void Window::scrollCallback(GLFWwindow* w, double, double yoffset) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (self) self->scroll_ += (float)yoffset;
}