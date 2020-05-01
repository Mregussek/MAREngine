
#ifndef CAMERA_H
#define CAMERA_H

#include "../mar.h"
#include "../Devices/SerialPortMonitor.h"

namespace mar {
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    enum class ObjectRotation {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        NONE
    };

    struct {
        // --- Default Values for Camera
        const float YAW = -90.0f;
        const float PITCH = 0.0f;
        const float SPEED = 2.5f;
        const float SENSITIVITY = 0.1f;
        const float ZOOM = 45.0f;
    } CameraSettings;

    class Camera {
        // --- Camera Attributes
        glm::vec3 _position;
        glm::vec3 _front;
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _worldUp;
        // --- Mouse Settings
        float _movementSpeed;
        float _mouseSensitivity;
        float _zoom;
        // --- Maths (Euler Angles)
        float _yaw;
        float _pitch;
        // --- RealTime Operations
        float _deltaTime;
        float _lastFrame;
        ObjectRotation _objectRotation;

    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = CameraSettings.YAW, float pitch = CameraSettings.PITCH);

        const float& getZoom() const { return _zoom; }
        glm::mat4 getViewMatrix() { return glm::lookAt(_position, _position + _front, _up); }
        glm::mat4 getRotateMatrix(const glm::vec3& cubePosition);

        void processInput(GLFWwindow* window);
        void processKeyboard(CameraMovement&& direction);
        void processKeyboard(ObjectRotation&& direction);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void processMouseScroll(float yoffset);

    private:
        void updateCameraVectors();
    };
}

#endif // !CAMERA_H