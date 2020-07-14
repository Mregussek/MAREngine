/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_CAMERA_H
#define MAR_ENGINE_CAMERA_H

#include "../../mar.h"
#include "../../Window/Input.h"


namespace mar {
    namespace graphics {
        

		enum class CameraMovement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UP,
			DOWN
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
			const static maths::vec3 CAMERA_START;
		};

		struct CameraData {
			maths::mat4 projection;
			maths::mat4 view;
			maths::mat4 model;
			maths::mat4 mvp;
			maths::vec3 position;

			CameraData() = default;
		};

		class Camera {
			// --- Window Attributes
			const float* m_windowWidth;
			const float* m_windowHeight;
			// --- Camera Attributes
			maths::vec3 m_position;
			maths::vec3 m_front;
			maths::vec3 m_up;
			maths::vec3 m_right;
			maths::vec3 m_worldUp;
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
			// --- References
			const float* m_mouseCallX;
			const float* m_mouseCallY;
			const float* m_scrollCallX;
			const float* m_scrollCallY;
			// --- Data for renderer
			static CameraData s_cameraData;

		public:
			Camera();

			void initialize();

			void processInput();
			void updateData();

			void mouseButtonCallback(float xpos, float ypos);
			void mouseCallback(float xpos, float ypos);
			void scrollCallback(float ypos);

		private:
			void processCameraMovement(CameraMovement&& direction);
			void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
			void processMouseScroll(float yoffset);
			void updateCameraVectors();

		public:
			// --- SET METHODS --- //
			void setWindowSize(const float* width, const float* height) { 
				m_windowHeight = height; 
				m_windowWidth = width; 
			}

			void setMouseCall(const float* x, const float* y) {
				m_mouseCallX = x;
				m_mouseCallY = y;
			}

			void setScrollCall(const float* x, const float* y) {
				m_scrollCallX = x;
				m_scrollCallY = y;
			}

			// --- GET METHODS --- //
			inline static CameraData& getCameraData() { return s_cameraData; }

			inline const maths::vec3& getCameraPosition() const { return m_position; }
			inline const float& getZoom() const { return m_zoom; }
			inline const maths::mat4 getProjectionMatrix() const 
			{ return maths::mat4::perspective(maths::Trig::toRadians(m_zoom), *m_windowWidth / *m_windowHeight, 0.01f, 100.0f); }
			inline const maths::mat4 getViewMatrix() const 
			{ return maths::mat4::lookAt(m_position, m_position + m_front, m_up); }
			inline const maths::mat4 getModelMatrix() const 
			{ return maths::mat4::translation({ 0.0f, 0.0f, 0.0f }); }
		};


} }

#endif // !MAR_ENGINE_CAMERA_H