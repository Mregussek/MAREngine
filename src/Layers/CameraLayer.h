/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_CAMERA_H
#define MAR_ENGINE_LAYER_CAMERA_H


#include "../mar.h"
#include "Layer.h"
#include "../Core/Camera/Camera.h"
#include "../Window/Window.h"


namespace mar {
	namespace layers {


		class CameraLayer : public Layer {
			const char* m_debugName;
			graphics::Camera* m_camera;

		public:

			CameraLayer(const char* name) : m_debugName(name) {}

			void initialize() {
				m_camera = new graphics::Camera();

				m_camera->initialize();
			}

			void prepareFrame() override {

			}

			void update() override {
				m_camera->processInput();
				m_camera->updateData();
			}
			
			void endFrame() override {

			}

			void closeLayer() override {
				delete m_camera;
			}

		public:
			// --- GET METHODS --- //
			inline const graphics::CameraData& getCameraData() const { return m_camera->getCameraData(); }

			// --- SET METHODS --- //
			void set(const int& win_h, const int& win_w) { m_camera->setWindowSize((const float*)&win_h, (const float*)&win_w); }
			void set(const float& win_h, const float& win_w) { m_camera->setWindowSize(&win_h, &win_w);	}

			void mouseSetup() {
				auto win = &window::Window::getInstance();

				m_camera->setMouseCall(&win->getMouseX(), &win->getMouseY());
				m_camera->setScrollCall(&win->getScrollX(), &win->getScrollY());
			}
		};


} }


#endif // !MAR_ENGINE_LAYER_CAMERA_H
