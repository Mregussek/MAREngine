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


    void FLayerStack::begin() {
        for (ILayer* pLayer : m_layers) {
            pLayer->begin();
        }
    }

	void FLayerStack::update() {
		for (ILayer* pLayer : m_layers) {
            pLayer->update();
        }
	}

    void FLayerStack::end() {
        for (ILayer* pLayer : m_layers) {
            pLayer->end();
        }
    }

	void FLayerStack::close() {
		for (ILayer* pLayer : m_layers) {
            pLayer->close();
		}
	}

	void FLayerStack::pushLayer(ILayer* pLayer) {
		m_layers.emplace(m_layers.begin() + m_insertValue, pLayer);
		m_insertValue++;
	}

	void FLayerStack::pushOverlay(ILayer* pOverlay) {
		m_layers.emplace_back(pOverlay);
	}

	void FLayerStack::popLayer(ILayer* pLayer) {
	    const auto itLayersEnd = m_layers.begin() + m_insertValue;

		auto it = std::find(m_layers.begin(), itLayersEnd, pLayer);
		if (it != itLayersEnd) {
			m_layers.erase(it);
            m_insertValue--;

			return;
		}
	}

	void FLayerStack::popOverlay(ILayer* pOverlay) {
		auto it = std::find(m_layers.begin() + m_insertValue, m_layers.end(), pOverlay);
		if (it != m_layers.end()) {
			m_layers.erase(it);
			return;
		}
	}


}
