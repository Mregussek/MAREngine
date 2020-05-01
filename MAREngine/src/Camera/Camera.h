
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
        const glm::vec3 CAMERA_START = glm::vec3(0.0f, 0.0f, 7.0f);
    } CameraSettings;

    class Camera {
        // --- Window Attributes
        int _width;
        int _height;
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
        bool _enableMouse;
        bool _firstMouse;
        float _lastX;
        float _lastY;
        // --- Maths (Euler Angles)
        float _yaw;
        float _pitch;
        // --- RealTime Operations
        float _deltaTime;
        float _lastFrame;
        ObjectRotation _objectRotation;

    public:
        Camera(const int& w, const int& h);

        const float& getZoom() const { return _zoom; }
        const glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(_zoom), (float)_width / (float)_height, 0.1f, 100.0f); }
        const glm::mat4 getViewMatrix() const { return glm::lookAt(_position, _position + _front, _up); }
        const glm::mat4 getRotateMatrixOnPress(const glm::vec3& cubePosition) const ;
        const glm::mat4 getRotateMatrixSPM(const glm::vec3& cubePosition, const glm::vec3& spmRotator) const;

        void processInput(GLFWwindow* window);

        void mouseCallback(float xpos, float ypos);  
        void scrollCallback(float ypos);

    private:
        void processKeyboard(CameraMovement&& direction);
        void processKeyboard(ObjectRotation&& direction);
        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void processMouseScroll(float yoffset);
        void updateCameraVectors();
    };
}

#endif // !CAMERA_H