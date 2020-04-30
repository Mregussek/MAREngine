
#include "Camera.h"

namespace mar {
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : _position(position),
        _front(glm::vec3(0.0f, 0.0f, -1.0f)),
        _worldUp(up),
        _movementSpeed(CameraSettings.SPEED),
        _mouseSensitivity(CameraSettings.SENSITIVITY),
        _zoom(CameraSettings.ZOOM),
        _yaw(yaw),
        _pitch(pitch),
        _deltaTime(0.0f),
        _lastFrame(0.0f)
    {
        updateCameraVectors();
    }

    void Camera::processInput(GLFWwindow* window) {
        float currentFrame = (float)glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            processKeyboard(CameraMovement::FORWARD, _deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            processKeyboard(CameraMovement::BACKWARD, _deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            processKeyboard(CameraMovement::LEFT, _deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            processKeyboard(CameraMovement::RIGHT, _deltaTime);
    }

    void Camera::processKeyboard(CameraMovement&& direction, float deltaTime) {
        float velocity = _movementSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD) _position += _front * velocity;
        if (direction == CameraMovement::BACKWARD) _position -= _front * velocity;
        if (direction == CameraMovement::LEFT) _position -= _right * velocity;
        if (direction == CameraMovement::RIGHT) _position += _right * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
        xoffset *= _mouseSensitivity;
        yoffset *= _mouseSensitivity;

        _yaw += xoffset;
        _pitch += yoffset;

        if (constrainPitch) {
            if (_pitch > 89.0f) _pitch = 89.0f;
            if (_pitch < -89.0f)_pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset) {
        if (_zoom >= 1.0f && _zoom <= 45.0f) _zoom -= yoffset;
        if (_zoom <= 1.0f) _zoom = 1.0f;
        if (_zoom >= 45.0f) _zoom = 45.0f;
    }

    void Camera::updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);
        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));
    }
}