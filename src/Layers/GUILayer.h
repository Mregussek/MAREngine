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
			std::string m_debugName;

			gui::GUI* m_gui;

		public:
			GUILayer(std::string name) : m_debugName(name) { }

			void set(const graphics::RendererStatistics* stats, const Ref<graphics::FrameBuffer>& framebuf = nullptr) {
				m_gui->setReferences(stats);

				if (framebuf != nullptr)
					m_gui->setFrameBuffer(framebuf);
			}

			void submit(graphics::Mesh* mesh) {
				m_gui->submitMesh(mesh);
			}

			void update() override {
				m_gui->prepareNewFrame();
				m_gui->display();
			}
		};


} }


#endif // !MAR_ENGINE_GUI_LAYER_H
