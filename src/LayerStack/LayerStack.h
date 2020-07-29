/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_STACK_H
#define MAR_ENGINE_LAYER_STACK_H


#include "../mar.h"
#include "Layer.h"
#include "../Debug/Log.h"


namespace mar {
	namespace layers {


		class LayerStack {
			std::vector<Layer*> m_layers;
			uint32_t m_layerInsert{ 0 };

		public:
			LayerStack() {
				MAR_CORE_INFO("Initialized Layer Stack!");
			}

			Layer* operator[](const uint32_t& index) { return m_layers[index]; }

			void close() {
				for (auto& layer : m_layers) {
					layer->closeLayer();
					delete layer;
				}

				MAR_CORE_INFO("Closed Layer Stack!");
			}

			void update() { 
				MAR_CORE_TRACE("STACK: updating frame");

				for (auto& layer : m_layers) 
					layer->update();
			}

			void pushLayer(Layer* layer) {
				m_layers.emplace(m_layers.begin() + m_layerInsert, layer);
				m_layerInsert++;

				MAR_CORE_INFO("Pushed layer to stack!");
			}

			void pushOverlay(Layer* overlay) {
				m_layers.emplace_back(overlay);

				MAR_CORE_INFO("Pushed Overlay to stack!");
			}

			void popLayer(Layer* layer) {
				auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsert, layer);
				if (it != m_layers.begin() + m_layerInsert) {
					m_layers.erase(it);
					m_layerInsert--;

					MAR_CORE_INFO("Popped layer from stack");

					return;
				}

				MAR_CORE_ERROR("Cannot pop layer from stack");
			}

			void popOverlay(Layer* overlay) {
				auto it = std::find(m_layers.begin() + m_layerInsert, m_layers.end(), overlay);
				if (it != m_layers.end()) {
					m_layers.erase(it);

					MAR_CORE_INFO("Popped overlay from stack!");

					return;
				}

				MAR_CORE_ERROR("Cannot pop overlay from stack!");
			}

		};


} }


#endif // !MAR_ENGINE_LAYER_STACK_H
