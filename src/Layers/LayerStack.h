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
			std::vector<Layer*>::iterator m_layerInsert;

		public:
			LayerStack() {
				m_layerInsert = m_layers.begin();
			}

			void prepareFrame() {
			
			}

			void updateFrame() { 
			
			}

			void endFrame() { 
			
			}

			void pushLayer(Layer* layer) {
				m_layerInsert = m_layers.emplace(m_layerInsert, layer);
			}

			void pushOverlay(Layer* overlay) {
				m_layers.emplace_back(overlay);
			}

			void popLayer(Layer* layer) {
				auto it = std::find(m_layers.begin(), m_layers.end(), layer);
				if (it != m_layers.end()) {
					m_layers.erase(it);
					m_layerInsert--;
				}
			}

			void popOverlay(Layer* overlay) {
				auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
				if (it != m_layers.end()) {
					m_layers.erase(it);
				}
			}

			void flush() {

			}

		};


} }


#endif // !MAR_ENGINE_LAYER_STACK_H

