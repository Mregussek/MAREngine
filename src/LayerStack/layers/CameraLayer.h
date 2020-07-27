/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_CAMERA_H
#define MAR_ENGINE_LAYER_CAMERA_H


#include "../../mar.h"
#include "../Layer.h"
#include "../../Core/Camera/Camera.h"
#include "../../Window/Window.h"


namespace mar {
	namespace layers {


		class CameraLayer : public Layer {
			const char* m_debugName;
			graphics::Camera* m_camera;

		public:

			CameraLayer(const char* name);

			void initialize();

			void mouseSetup();

			// --- OVERLOADED METHODS --- //

			void update() override;

			void closeLayer() override;

			// --- GET METHODS --- //

			inline graphics::Camera* getCamera() { return m_camera; }
		};


} }


#endif // !MAR_ENGINE_LAYER_CAMERA_H
