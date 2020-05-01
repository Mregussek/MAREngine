
#include "Camera.h"

namespace mar {
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : _position(position),
        _front(glm::vec3(0.0f, 0.0f, -1.0f)),
        _worldUp(up),
        _movementSpeed(CameraSettings.SPEED),
        _mouseSensitivity(CameraSettings.SENSITIVITY),
        _zoom(CameraSettings.ZOOM),
        _enableMouse(false),
        _firstMouse(false),
        _lastX(0.0f),
        _lastY(0.0f),
        _yaw(yaw),
        _pitch(pitch),
        _deltaTime(0.0f),
        _lastFrame(0.0f)
    {
        updateCameraVectors();
    }

    glm::mat4 Camera::getRotateMatrixOnPress(const glm::vec3& cubePosition) {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePosition);
        if (_objectRotation == ObjectRotation::FORWARD)
            return glm::rotate(transform, (float)glfwGetTime(), glm::vec3(-1.0f, 0.0f, 0.0f));
        else if (_objectRotation == ObjectRotation::BACKWARD)
            return glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        else if (_objectRotation == ObjectRotation::LEFT)
            return glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, -1.0f, 0.0f));
        else if (_objectRotation == ObjectRotation::RIGHT)
            return glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        else
            return transform;
    }

    glm::mat4 Camera::getRotateMatrixSPM(const glm::vec3& cubePosition, const glm::vec3& spmRotator) {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePosition);
        return glm::rotate(transform, (float)glfwGetTime(), spmRotator);
    }

    void Camera::processInput(GLFWwindow* window) {
        float currentFrame = (float)glfwGetTime();
        _deltaTime = currentFrame - _lastFrame;
        _lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Camera move check
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            processKeyboard(CameraMovement::FORWARD);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            processKeyboard(CameraMovement::BACKWARD);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            processKeyboard(CameraMovement::LEFT);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            processKeyboard(CameraMovement::RIGHT);

        // Rotation check
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            processKeyboard(ObjectRotation::FORWARD);
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            processKeyboard(ObjectRotation::BACKWARD);
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            processKeyboard(ObjectRotation::RIGHT);
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            processKeyboard(ObjectRotation::LEFT);
        else
            processKeyboard(ObjectRotation::NONE);

        // Enable Mouse Usage
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            if (_enableMouse) _enableMouse = false;
            else _enableMouse = true;

            _firstMouse = true;
        }    
    }

    void Camera::processKeyboard(CameraMovement&& direction) {
        float velocity = _movementSpeed * _deltaTime;
        if (direction == CameraMovement::FORWARD) _position += _front * velocity;
        if (direction == CameraMovement::BACKWARD) _position -= _front * velocity;
        if (direction == CameraMovement::LEFT) _position -= _right * velocity;
        if (direction == CameraMovement::RIGHT) _position += _right * velocity;
    }

    void Camera::processKeyboard(ObjectRotation&& rotation) {
        if (rotation == ObjectRotation::FORWARD) _objectRotation = ObjectRotation::FORWARD;
        if (rotation == ObjectRotation::BACKWARD) _objectRotation = ObjectRotation::BACKWARD;
        if (rotation == ObjectRotation::LEFT) _objectRotation = ObjectRotation::LEFT;
        if (rotation == ObjectRotation::RIGHT) _objectRotation = ObjectRotation::RIGHT;
        if (rotation == ObjectRotation::NONE) _objectRotation = ObjectRotation::NONE;
    }

    void Camera::mouseCallback(float xpos, float ypos) {
        if (!_enableMouse) return;

        if (_firstMouse) {
            _lastX = (float)xpos;
            _lastY = (float)ypos;
            _firstMouse = false;
        }

        float xoffset = (float)xpos - _lastX;
        float yoffset = _lastY - (float)ypos;

        _lastX = (float)xpos;
        _lastY = (float)ypos;

        processMouseMovement(xoffset, yoffset);
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
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

    void Camera::scrollCallback(float ypos) {
        processMouseScroll(ypos);
    }

    void Camera::processMouseScroll(float yoffset) {
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