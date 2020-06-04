/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "../../mar.h"
#include "buttons_def.h"

namespace mar {
    namespace graphics {
        

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
			RIGHT
		};

		struct {
			// --- Default Values for Camera
			const float YAW = -90.0f;
			const float PITCH = 0.0f;
			const float SPEED = 5.0f;
			const float SENSITIVITY = 0.1f;
			const float ZOOM = 55.0f;
			const glm::vec3 CAMERA_START = glm::vec3(0.0f, 0.0f, 7.0f);
		} CameraSettings;

		struct CameraData {
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;
			glm::vec3 position;

			CameraData() = default;
		};

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
			// --- Data for renderer
			CameraData _cameraData;

		public:
			Camera();

			void initialize(const int& w, const int& h);

			void processInput(GLFWwindow* window);
			void updateData();

			void mouseCallback(float xpos, float ypos);
			void scrollCallback(float ypos);

		private:
			void processKeyboard(CameraMovement&& direction);
			void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
			void processMouseScroll(float yoffset);
			void updateCameraVectors();

		public:
			const glm::vec3& getCameraPosition() const { return _position; }
			const CameraData& getCameraData() const { return _cameraData; }
			const float& getZoom() const { return _zoom; }
			const glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(_zoom), (float)_width / (float)_height, 0.01f, 100.0f); }
			const glm::mat4 getViewMatrix() const { return glm::lookAt(_position, _position + _front, _up); }
			const glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f }); }
		};


} }

#endif // !CAMERA_H