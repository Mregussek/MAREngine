/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "LayerStack.h"
#include "Layer.h"
#include "LayerLogs.h"


namespace mar::layers {


	LayerStack::LayerStack() { LAYER_INFO("LAYER_STACK: initialized (called default constructor)!"); }

	Layer* LayerStack::operator[](uint32_t index) { return m_layers[index]; }

	void LayerStack::initialize() {
		LAYER_TRACE("LAYER_STACK: going to call initialize() on layers");

		for (auto& layer : m_layers)
			layer->initialize();

		LAYER_INFO("LAYER_STACK: initialized layers");
	}

	void LayerStack::update() {
		LAYER_TRACE("LAYER_STACK: going to call update() on layers");

		for (auto& layer : m_layers)
			layer->update();

		LAYER_INFO("LAYER_STACK: updated layers");
	}

	void LayerStack::close() {
		LAYER_TRACE("LAYER_STACK: going to delete all layers!");

		for (auto& layer : m_layers) {
			layer->closeLayer();
			delete layer;
		}

		LAYER_INFO("LAYER_STACK: closed deleted all layers!");
	}

	void LayerStack::pushLayer(Layer* layer) {
		LAYER_TRACE("LAYER_STACK: Going to push layer at {}!", m_layerInsert);

		m_layers.emplace(m_layers.begin() + m_layerInsert, layer);
		m_layerInsert++;

		LAYER_INFO("LAYER_STACK: Pushed layer to stack at {}!", m_layerInsert);
	}

	void LayerStack::pushOverlay(Layer* overlay) {
		m_layers.emplace_back(overlay);

		LAYER_TRACE("LAYER_STACK: Pushed Overlay to stack!");
	}

	void LayerStack::popLayer(Layer* layer) {
		LAYER_TRACE("LAYER_STACK: going to pop layer from stack - {}", layer->m_debugName);

		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsert, layer);
		if (it != m_layers.begin() + m_layerInsert) {
			delete* it;

			m_layers.erase(it);
			m_layerInsert--;

			LAYER_INFO("LAYER_STACK: Popped layer from stack!");

			return;
		}

		LAYER_ERROR("LAYER_STACK: cannot find layer on the stack, so it cannot be deleted - {}!", layer->m_debugName);
	}

	void LayerStack::popOverlay(Layer* overlay) {
		LAYER_TRACE("LAYER_STACK: going to pop overlay from stack - {}", overlay->m_debugName);

		auto it = std::find(m_layers.begin() + m_layerInsert, m_layers.end(), overlay);
		if (it != m_layers.end()) {
			m_layers.erase(it);

			LAYER_INFO("LAYER_STACK: Popped overlay from stack!");

			return;
		}

		LAYER_ERROR("LAYER_STACK: cannot find overlay on the stack, so it cannot be deleted - {}!", overlay->m_debugName);
	}


}
