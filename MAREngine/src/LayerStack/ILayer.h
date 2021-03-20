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


#ifndef MARENGINE_ILAYER_H
#define MARENGINE_ILAYER_H


namespace marengine {


    /**
     * @class ILayer ILayer.h "LayerStack/ILayer.h"
     * @brief ILayer is an interface for all layers. Used especially as a convention plan
     * so that LayerStack can use any implementation of layer.
     */
	class ILayer {
	public:

        /// @brief Should be interpreted as beginFrame method. Called every frame before update().
        virtual void begin() = 0;

        /// @brief Update Frame method. Called every frame.
        virtual void update() = 0;

        /// @brief End Frame method. Called every frame after update().
		virtual void end() = 0;

		/// @brief Implementations should terminate all layers, its members and so on. Called once at the end of engine run.
		virtual void close() = 0;

	};


}


#endif // !MAR_ENGINE_LAYER_H
