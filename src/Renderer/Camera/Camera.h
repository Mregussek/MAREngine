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

		struct CameraSettings {
			// --- Default Values for Camera
			const static float YAW;
			const static float PITCH;
			const static float SPEED;
			const static float SENSITIVITY;
			const static float ZOOM;
			const static glm::vec3 CAMERA_START;
		};

		struct CameraData {
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;
			glm::vec3 position;

			CameraData() = default;
		};

		class Camera {
			// --- Window Attributes
			float m_windowWidth;
			float m_windowHeight;
			// --- Camera Attributes
			glm::vec3 m_position;
			glm::vec3 m_front;
			glm::vec3 m_up;
			glm::vec3 m_right;
			glm::vec3 m_worldUp;
			// --- Mouse Settings
			float m_movementSpeed;
			float m_mouseSensitivity;
			float m_zoom;
			bool m_enableMouse;
			bool m_firstMouse;
			float m_lastX;
			float m_lastY;
			// --- Maths (Euler Angles)
			float m_yaw;
			float m_pitch;
			// --- RealTime Operations
			float m_deltaTime;
			float m_lastFrame;
			// --- Data for renderer
			CameraData m_cameraData;
			// --- Window Reference
			GLFWwindow* m_window;

		public:
			Camera();

			void initialize(const float& w, const float& h);

			void processInput();
			void updateData();

			void mouseCallback(float xpos, float ypos);
			void scrollCallback(float ypos);

		private:
			void processKeyboard(CameraMovement&& direction);
			void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
			void processMouseScroll(float yoffset);
			void updateCameraVectors();

		public:
			// --- SET METHODS --- //
			void setReference(GLFWwindow* window) { m_window = window; }
			void setWindowSize(const float& width, const float& height) { m_windowHeight = height; m_windowWidth = width; }

			// --- GET METHODS --- //
			inline const CameraData& getCameraData() const { return m_cameraData; }

			inline const glm::vec3& getCameraPosition() const { return m_position; }
			inline const float& getZoom() const { return m_zoom; }
			inline const glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(m_zoom), m_windowWidth / m_windowHeight, 0.01f, 100.0f); }
			inline const glm::mat4 getViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }
			inline const glm::mat4 getModelMatrix() const { return glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f }); }
		};


} }

#endif // !CAMERA_H