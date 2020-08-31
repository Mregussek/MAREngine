/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_LAYER_H
#define MAR_ENGINE_GUI_LAYER_H

#include "../../mar.h"
#include "../Layer.h"
#include "../../Window/Window.h"



namespace mar {
	namespace editor { class Camera; class GUI; }


	namespace layers {


		class LayerGUI : public Layer {
			const char* m_debugName;
			editor::GUI* m_gui;
			editor::Camera* m_camera;

		public:
			LayerGUI() = default;
			LayerGUI(const char* name);

			void initialize(editor::GUI* gui, editor::Camera* cam);

			// --- OVERLOADED METHODS --- // 

			void update() override;
			void closeLayer() override;

			// --- GET METHODS --- //

			inline editor::GUI* getGUIInstance();
			inline editor::Camera* getCamera();
		};


} }


#endif // !MAR_ENGINE_GUI_LAYER_H
