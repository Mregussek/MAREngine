
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

    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = CameraSettings.YAW, float pitch = CameraSettings.PITCH);

        const float& getZoom() const { return _zoom; }
        glm::mat4 GetViewMatrix() { return glm::lookAt(_position, _position + _front, _up); }

        void processInput(GLFWwindow* window);
        void processKeyboard(CameraMovement&& direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void ProcessMouseScroll(float yoffset);

    private:
        void updateCameraVectors();
    };
}

#endif // !CAMERA_H