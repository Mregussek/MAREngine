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


#ifndef MARENGINE_LAYERSTACK_H
#define MARENGINE_LAYERSTACK_H


#include "../mar.h"


namespace marengine {

	class ILayer;


	class FLayerStack {
	public:

        void update();
		void close();

		void pushLayer(ILayer* pLayer);
        void popLayer(ILayer* pLayer);

		void pushOverlay(ILayer* pOverlay);
		void popOverlay(ILayer* pOverlay);

	private:

		std::vector<ILayer*> m_layers;
		uint32_t m_insertValue{ 0 };

	};


}


#endif // !MARENGINE_LAYERSTACK_H

