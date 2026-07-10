#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

// Orbit / isometric camera. Produces its own view + projection matrices and
// supports pan, zoom, and 90-degree snap rotation. It is intentionally free of
// any windowing code — feed it an aspect ratio and input deltas from whatever
// frontend owns the GL context (the GLFW runtime, a Qt editor viewport, ...).
class Camera {
public:
    glm::vec3 target = glm::vec3(0.0f);   // point the camera looks at / orbits
    float aspect     = 16.0f / 9.0f;      // viewport width / height
    float zoom       = 6.0f;              // ortho half-height (also perspective arm)
    float distance   = 50.0f;             // arm length in ortho mode
    float pitch      = glm::radians(35.0f);
    float yaw        = glm::radians(45.0f);
    float targetYaw  = glm::radians(45.0f);
    bool  usePerspective = false;         // false = ortho, true = perspective

    // Tunables (were Config:: constants in the source engine).
    float rotateSpeed = 10.0f;
    float panSpeed    = 0.8f;
    float zoomMin     = 2.0f;
    float zoomMax     = 240.0f;
    float fov         = 40.0f;
    float nearOrtho   = -200.0f, farOrtho = 200.0f;
    float nearPersp   = 0.1f,    farPersp = 500.0f;

    // Queue a 90-degree snap; update() eases toward it.
    void rotateLeft()  { targetYaw += glm::radians(90.0f); }
    void rotateRight() { targetYaw -= glm::radians(90.0f); }

    // Continuous rotate (e.g. mouse drag) with no snap.
    void dragYaw(float delta) { yaw += delta; targetYaw = yaw; }

    // Scroll-wheel zoom, clamped.
    void zoomBy(float delta) { zoom = glm::clamp(zoom - delta, zoomMin, zoomMax); }

    // Smoothly ease yaw toward targetYaw. Call once per frame.
    void update(float dt) {
        float t = 1.0f - std::exp(-rotateSpeed * dt);
        yaw = glm::mix(yaw, targetYaw, t);
    }

    // Move the target in the ground plane, relative to the current yaw.
    void pan(float forward, float right, float dt) {
        float speed   = zoom * panSpeed * dt;
        glm::vec3 fwd = { std::sin(yaw), 0.0f,  std::cos(yaw) };
        glm::vec3 rgt = { std::cos(yaw), 0.0f, -std::sin(yaw) };
        target += (fwd * forward + rgt * right) * speed;
    }

    glm::mat4 getView() const {
        float arm = usePerspective ? zoom : distance;
        glm::vec3 offset(
            arm * std::cos(pitch) * std::sin(yaw),
            arm * std::sin(pitch),
            arm * std::cos(pitch) * std::cos(yaw));
        return glm::lookAt(target + offset, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 getProjection() const {
        if (usePerspective)
            return glm::perspective(glm::radians(fov), aspect, nearPersp, farPersp);
        return glm::ortho(-zoom * aspect, zoom * aspect,
                          -zoom,           zoom,
                          nearOrtho,       farOrtho);
    }
};
