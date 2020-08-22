/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_STACK_H
#define MAR_ENGINE_LAYER_STACK_H


#include "../mar.h"
#include "Layer.h"
#include "LayerLogs.h"


namespace mar {
	namespace layers {


		class LayerStack {
			std::vector<Layer*> m_layers;
			uint32_t m_layerInsert{ 0 };

		public:
			LayerStack() { LAYER_INFO("LAYER_STACK: initialized (called default constructor)!"); }

			Layer* operator[](const uint32_t& index) { return m_layers[index]; }

			void close() {
				LAYER_TRACE("LAYER_STACK: going to delete all layers!");

				for (auto& layer : m_layers) {
					layer->closeLayer();
					delete layer;
				}

				LAYER_INFO("LAYER_STACK: closed deleted all layers!");
			}

			void update() { 
				LAYER_TRACE("LAYER_STACK: going to call update() on layers");

				for (auto& layer : m_layers) 
					layer->update();

				LAYER_INFO("LAYER_STACK: updated layers");
			}

			void pushLayer(Layer* layer) {
				LAYER_TRACE("LAYER_STACK: Going to push layer at {}!", m_layerInsert);

				m_layers.emplace(m_layers.begin() + m_layerInsert, layer);
				m_layerInsert++;

				LAYER_INFO("LAYER_STACK: Pushed layer to stack at {}!", m_layerInsert);
			}

			void pushOverlay(Layer* overlay) {
				m_layers.emplace_back(overlay);

				LAYER_TRACE("LAYER_STACK: Pushed Overlay to stack!");
			}

			void popLayer(Layer* layer) {
				LAYER_TRACE("LAYER_STACK: going to pop layer from stack - {}", layer->m_debugName);

				auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsert, layer);
				if (it != m_layers.begin() + m_layerInsert) {
					delete *it;
					
					m_layers.erase(it);
					m_layerInsert--;

					LAYER_INFO("LAYER_STACK: Popped layer from stack!");

					return;
				}

				LAYER_ERROR("LAYER_STACK: cannot find layer on the stack, so it cannot be deleted - {}!", layer->m_debugName);
			}

			void popOverlay(Layer* overlay) {
				LAYER_TRACE("LAYER_STACK: going to pop overlay from stack - {}", overlay->m_debugName);

				auto it = std::find(m_layers.begin() + m_layerInsert, m_layers.end(), overlay);
				if (it != m_layers.end()) {
					m_layers.erase(it);

					LAYER_INFO("LAYER_STACK: Popped overlay from stack!");

					return;
				}

				LAYER_ERROR("LAYER_STACK: cannot find overlay on the stack, so it cannot be deleted - {}!", overlay->m_debugName);
			}

		};


} }


#endif // !MAR_ENGINE_LAYER_STACK_H

