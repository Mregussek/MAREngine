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


#include <vector>


namespace marengine {

	class ILayer;


	/**
	 * @class FLayerStack LayerStack.h "LayerStack/LayerStack.h"
	 * @brief FLayerStack is a class responsible for layers management in MAREngine. It operates as a stack
	 * and with this engine can push layer and then during every frame call its begin / update / end methods.
	 */
	class FLayerStack {
	public:

	    /// @brief Iterates over all layers stored in stack and calls its begin() method.
	    void begin();

        /// @brief Iterates over all layers stored in stack and calls its update() method.
        void update();

        /// @brief Iterates over all layers stored in stack and calls its end() method.
        void end();

        /// @brief Iterates over all layers stored in stack and calls its close() method.
		void close();

		/**
		 * @brief Push already existing layer to stack.
		 * @param pLayer Layer that will be emplaced at stack
		 */
		void pushLayer(ILayer* pLayer);

		/**
		 * @brief Pop already existing layer from stack. It looks, if given layer exists at stack
		 * and then removes it.
		 * @param pLayer Layer that will be removed from stack
		 */
        void popLayer(ILayer* pLayer);

        /**
         * @brief Push already existing layer to stack and make it as overlay (place it at the end of the stack).
         * @param pOverlay Layer that will be emplaced at stack as overlay.
         */
		void pushOverlay(ILayer* pOverlay);

		/**
		 * @brief Pop already existing overlay from stack.
		 * @param pOverlay Layer that will removed from stack.
		 */
		void popOverlay(ILayer* pOverlay);

	private:

		std::vector<ILayer*> m_layers;
		uint32_t m_insertValue{ 0 };

	};


}


#endif // !MARENGINE_LAYERSTACK_H

