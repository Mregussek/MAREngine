/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Camera.h"
#include "../EditorLogging.h"

namespace mar {
    namespace editor {


		const float CameraSettings::YAW = 0.0f;
		const float CameraSettings::PITCH = 0.0f;
		const float CameraSettings::SPEED = 5.0f;
		const float CameraSettings::SENSITIVITY = 0.1f;
		const float CameraSettings::ZOOM = 55.0f;
		const maths::vec3 CameraSettings::CAMERA_START{ -10.0f, 2.0f, 0.0f };

		CameraData Camera::s_cameraData;

		Camera::Camera()
			: m_position(CameraSettings::CAMERA_START),
			m_front({ -1.0f, 0.0f, 0.0f }),
			m_worldUp({ 0.0f, 1.0f, 0.0f }),
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

		void Camera::initialize() {
			updateCameraVectors();

			MAR_CORE_INFO("Camera has been initialized successfully!");
		}

		void Camera::processInput() {
			static float currentFrame;

			currentFrame = (float)glfwGetTime();
			m_deltaTime = currentFrame - m_lastFrame;
			m_lastFrame = currentFrame;

			if (window::Input::isKeyPressed(MAR_KEY_W))
				processCameraMovement(CameraMovement::FORWARD);
			if (window::Input::isKeyPressed(MAR_KEY_S))
				processCameraMovement(CameraMovement::BACKWARD);
			if (window::Input::isKeyPressed(MAR_KEY_A))
				processCameraMovement(CameraMovement::LEFT);
			if (window::Input::isKeyPressed(MAR_KEY_D))
				processCameraMovement(CameraMovement::RIGHT);

			if (window::Input::isKeyPressed(MAR_KEY_Q)) {
				if (m_enableMouse) m_enableMouse = false;
				else m_enableMouse = true;

				m_firstMouse = true;
			}

			mouseCallback(*m_mouseCallX, *m_mouseCallY);
			processMouseScroll(*m_scrollCallY);
		}

		void Camera::updateData() {
			s_cameraData.projection = getProjectionMatrix();
			s_cameraData.model = getModelMatrix();
			s_cameraData.view = getViewMatrix();

			s_cameraData.mvp = s_cameraData.projection * s_cameraData.view;// *s_cameraData.model;

			s_cameraData.position = getCameraPosition();
		}

		void Camera::processCameraMovement(CameraMovement&& direction) {
			static float velocity;

			velocity = m_movementSpeed * m_deltaTime;

			switch (direction) {
			case CameraMovement::FORWARD:
				m_position += m_front * velocity;		break;
			case CameraMovement::BACKWARD:
				m_position -= m_front * velocity;		break;
			case CameraMovement::LEFT:
				m_position -= m_right * velocity;		break;
			case CameraMovement::RIGHT:
				m_position += m_right * velocity;		break;
			case CameraMovement::UP:
				m_position += m_up * velocity;			break;
			case CameraMovement::DOWN:
				m_position -= m_up * velocity;		break;
			}
		}

		void Camera::mouseButtonCallback(float xpos, float ypos) {
			static float xoffset;
			static float yoffset;

			xoffset = xpos - m_lastX;
			yoffset = m_lastY - ypos;

			m_lastX = xpos;
			m_lastY = ypos;

			processMouseMovement(xoffset, yoffset);
		}

		void Camera::mouseCallback(float xpos, float ypos) {
			static float xoffset;
			static float yoffset;
			
			if (!m_enableMouse) return;

			if (m_firstMouse) {
				m_lastX = xpos;
				m_lastY = ypos;
				m_firstMouse = false;
			}

			xoffset = xpos - m_lastX;
			yoffset = m_lastY - ypos;

			m_lastX = xpos;
			m_lastY = ypos;

			processMouseMovement(xoffset, yoffset);
		}

		void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
			xoffset *= m_mouseSensitivity;
			yoffset *= m_mouseSensitivity;

			m_yaw += xoffset;
			m_pitch += yoffset;

			if (constrainPitch) {
				if (m_pitch >  89.0f) m_pitch = 89.0f;
				if (m_pitch < -89.0f) m_pitch = -89.0f;
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
			static maths::vec3 front;

			front.x = maths::Trig::cosine(maths::Trig::toRadians(m_yaw))* maths::Trig::cosine(maths::Trig::toRadians(m_pitch));
			front.y = maths::Trig::sine(maths::Trig::toRadians(m_pitch));
			front.z = maths::Trig::sine(maths::Trig::toRadians(m_yaw))* maths::Trig::cosine(maths::Trig::toRadians(m_pitch));

			m_front = maths::vec3::normalize(front);
			m_right = maths::vec3::normalize(maths::vec3::cross(m_front, m_worldUp));
			m_up = maths::vec3::normalize(maths::vec3::cross(m_right, m_front));
		}


} }