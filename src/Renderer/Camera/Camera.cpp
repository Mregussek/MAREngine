/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Camera.h"

namespace mar {
    namespace graphics {


		Camera::Camera()
			: _width(0),
			_height(0),
			_position(CameraSettings.CAMERA_START),
			_front(glm::vec3(0.0f, 0.0f, -1.0f)),
			_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
			_movementSpeed(CameraSettings.SPEED),
			_mouseSensitivity(CameraSettings.SENSITIVITY),
			_zoom(CameraSettings.ZOOM),
			_enableMouse(false),
			_firstMouse(false),
			_lastX(0.0f),
			_lastY(0.0f),
			_yaw(CameraSettings.YAW),
			_pitch(CameraSettings.PITCH),
			_deltaTime(0.0f),
			_lastFrame(0.0f)
		{ }

		void Camera::initialize(const int& w, const int& h) {
			_width = w;
			_height = h;
			updateCameraVectors();
		}

		void Camera::processInput(GLFWwindow* window) {
			float currentFrame = (float)glfwGetTime();
			_deltaTime = currentFrame - _lastFrame;
			_lastFrame = currentFrame;

			if (glfwGetKey(window, MAR_KEY_ESCAPE) == MAR_KEY_IS_PRESSED)
				glfwSetWindowShouldClose(window, true);

			// Camera move check
			if (glfwGetKey(window, MAR_KEY_W) == MAR_KEY_IS_PRESSED)
				processKeyboard(CameraMovement::FORWARD);
			if (glfwGetKey(window, MAR_KEY_S) == MAR_KEY_IS_PRESSED)
				processKeyboard(CameraMovement::BACKWARD);
			if (glfwGetKey(window, MAR_KEY_A) == MAR_KEY_IS_PRESSED)
				processKeyboard(CameraMovement::LEFT);
			if (glfwGetKey(window, MAR_KEY_D) == MAR_KEY_IS_PRESSED)
				processKeyboard(CameraMovement::RIGHT);

			// Enable Mouse Usage
			if (glfwGetKey(window, MAR_KEY_1) == MAR_KEY_IS_PRESSED) {
				if (_enableMouse) _enableMouse = false;
				else _enableMouse = true;

				_firstMouse = true;
			}
		}

		void Camera::updateData() {
			_cameraData.projection = getProjectionMatrix();
			_cameraData.model = getModelMatrix();
			_cameraData.view = getViewMatrix();

			_cameraData.position = getCameraPosition();
		}

		void Camera::processKeyboard(CameraMovement&& direction) {
			float velocity = _movementSpeed * _deltaTime;

			switch (direction) {
			case CameraMovement::FORWARD:
				_position += _front * velocity;		break;
			case CameraMovement::BACKWARD:
				_position -= _front * velocity;		break;
			case CameraMovement::LEFT:
				_position -= _right * velocity;		break;
			case CameraMovement::RIGHT:
				_position += _right * velocity;		break;
			}
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


} }