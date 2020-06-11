/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Camera.h"

namespace mar {
    namespace graphics {


		const float CameraSettings::YAW = -90.0f;
		const float CameraSettings::PITCH = 0.0f;
		const float CameraSettings::SPEED = 5.0f;
		const float CameraSettings::SENSITIVITY = 0.1f;
		const float CameraSettings::ZOOM = 55.0f;
		const glm::vec3 CameraSettings::CAMERA_START = glm::vec3(0.0f, 0.0f, 7.0f);

		Camera::Camera()
			: m_windowWidth(0),
			m_windowHeight(0),
			m_position(CameraSettings::CAMERA_START),
			m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
			m_movementSpeed(CameraSettings::SPEED),
			m_mouseSensitivity(CameraSettings::SENSITIVITY),
			m_zoom(CameraSettings::ZOOM),
			m_enableMouse(false),
			m_firstMouse(false),
			m_lastX(0.0f),
			m_lastY(0.0f),
			m_yaw(CameraSettings::YAW),
			m_pitch(CameraSettings::PITCH),
			m_deltaTime(0.0f),
			m_lastFrame(0.0f)
		{ }

		void Camera::initialize(const int& w, const int& h) {
			m_windowWidth = w;
			m_windowHeight = h;
			updateCameraVectors();
		}

		void Camera::processInput(GLFWwindow* window) {
			float currentFrame = (float)glfwGetTime();
			m_deltaTime = currentFrame - m_lastFrame;
			m_lastFrame = currentFrame;

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
				if (m_enableMouse) m_enableMouse = false;
				else m_enableMouse = true;

				m_firstMouse = true;
			}
		}

		void Camera::updateData() {
			m_cameraData.projection = getProjectionMatrix();
			m_cameraData.model = getModelMatrix();
			m_cameraData.view = getViewMatrix();

			m_cameraData.position = getCameraPosition();
		}

		void Camera::processKeyboard(CameraMovement&& direction) {
			float velocity = m_movementSpeed * m_deltaTime;

			switch (direction) {
			case CameraMovement::FORWARD:
				m_position += m_front * velocity;		break;
			case CameraMovement::BACKWARD:
				m_position -= m_front * velocity;		break;
			case CameraMovement::LEFT:
				m_position -= m_right * velocity;		break;
			case CameraMovement::RIGHT:
				m_position += m_right * velocity;		break;
			}
		}

		void Camera::mouseCallback(float xpos, float ypos) {
			if (!m_enableMouse) return;

			if (m_firstMouse) {
				m_lastX = (float)xpos;
				m_lastY = (float)ypos;
				m_firstMouse = false;
			}

			float xoffset = (float)xpos - m_lastX;
			float yoffset = m_lastY - (float)ypos;

			m_lastX = (float)xpos;
			m_lastY = (float)ypos;

			processMouseMovement(xoffset, yoffset);
		}

		void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
			xoffset *= m_mouseSensitivity;
			yoffset *= m_mouseSensitivity;

			m_yaw += xoffset;
			m_pitch += yoffset;

			if (constrainPitch) {
				if (m_pitch > 89.0f) m_pitch = 89.0f;
				if (m_pitch < -89.0f)m_pitch = -89.0f;
			}

			updateCameraVectors();
		}

		void Camera::scrollCallback(float ypos) {
			processMouseScroll(ypos);
		}

		void Camera::processMouseScroll(float yoffset) {
			if (m_zoom >= 1.0f && m_zoom <= 45.0f) m_zoom -= yoffset;
			if (m_zoom <= 1.0f) m_zoom = 1.0f;
			if (m_zoom >= 45.0f) m_zoom = 45.0f;
		}

		void Camera::updateCameraVectors() {
			glm::vec3 front;

			front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			front.y = sin(glm::radians(m_pitch));
			front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

			m_front = glm::normalize(front);
			m_right = glm::normalize(glm::cross(m_front, m_worldUp));
			m_up = glm::normalize(glm::cross(m_right, m_front));
		}


} }