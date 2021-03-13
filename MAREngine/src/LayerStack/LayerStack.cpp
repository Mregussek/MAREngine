/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "LayerStack.h"
#include "Layer.h"


namespace marengine {


	Layer* LayerStack::operator[](uint32_t index) { return m_layers[index]; }

	void LayerStack::update() {
		for (auto& layer : m_layers) {
            layer->update();
        }
	}

	void LayerStack::close() {
		for (auto& layer : m_layers) {
			layer->close();
		}
	}

	void LayerStack::pushLayer(Layer* layer) {
		m_layers.emplace(m_layers.begin() + m_layerInsert, layer);
		m_layerInsert++;
	}

	void LayerStack::pushOverlay(Layer* overlay) {
		m_layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer* layer) {
		auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsert, layer);
		if (it != m_layers.begin() + m_layerInsert) {
			m_layers.erase(it);
			m_layerInsert--;

			return;
		}
	}

	void LayerStack::popOverlay(Layer* overlay) {
		auto it = std::find(m_layers.begin() + m_layerInsert, m_layers.end(), overlay);
		if (it != m_layers.end()) {
			m_layers.erase(it);
			return;
		}
	}


}
