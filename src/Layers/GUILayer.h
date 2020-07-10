/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_LAYER_H
#define MAR_ENGINE_GUI_LAYER_H

#include "../mar.h"
#include "../GUI/GUI.h"
#include "Layer.h"

namespace mar {
	namespace layers {


		class GUILayer : public Layer {
		protected:
			const char* m_debugName;

			gui::GUI* m_gui;

		public:
			GUILayer() = default;
			GUILayer(const char* name) : m_debugName(name) { }

			void initialize();
			void submit(graphics::Mesh* mesh);

			// --- OVERLOADED METHODS --- // 
			void prepareFrame() override { }
			void update() override;
			void endFrame() override { }
			void closeLayer() override;

			// --- SET METHODS --- //
			void set(const Ref<graphics::FrameBuffer>& framebuf = nullptr);
		};


} }


#endif // !MAR_ENGINE_GUI_LAYER_H
