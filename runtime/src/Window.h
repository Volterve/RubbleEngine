#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
	Window(const std::string& title, int width, int height);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool  isOpen() const;
    void  beginFrame();   // compute dt, poll events, clear the screen
    void  endFrame();     // swap buffers
    void  close();

    bool  key(int glfwKey) const;      // GLFW_KEY_W etc.
    float aspect() const;
    float deltaTime()   const { return dt_; }
    float scrollDelta() const { return scroll_; }
    GLFWwindow* handle() const { return window_; }

private:
    GLFWwindow* window_ = nullptr;
    int   width_ = 0, height_ = 0;
    float last_ = 0.0f, dt_ = 0.0f, scroll_ = 0.0f;
    float clearColor_[3] = { 0.10f, 0.11f, 0.13f };

    static void framebufferSizeCallback(GLFWwindow*, int, int);
    static void scrollCallback(GLFWwindow*, double, double);
};