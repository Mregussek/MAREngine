/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_CAMERA_H
#define MAR_ENGINE_LAYER_CAMERA_H


#include "../mar.h"
#include "Layer.h"
#include "../Renderer/Camera/Camera.h"


namespace mar {
	namespace layers {


		class CameraLayer : public Layer {
			graphics::Camera* m_camera;

		public:

			void initializeLayer(graphics::Camera* cam) {
				m_camera = cam;

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
			void set(const float* win_h, const float* win_w) {
				m_camera->setWindowSize(win_h, win_w);
			}

			void set(const window::Window* win) {
				m_camera->setMouseCall(&win->getMouseX(), &win->getMouseY());
				m_camera->setScrollCall(&win->getScrollX(), &win->getScrollY());
			}
		};


} }


#endif // !MAR_ENGINE_LAYER_CAMERA_H
