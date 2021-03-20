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


#ifndef MAR_ENGINE_LIGHT_COMPONENTS_H
#define MAR_ENGINE_LIGHT_COMPONENTS_H


#include "../../graphics/Lightning/LightDefinitions.h"


namespace marengine {


	/**
	 * @struct PointLightComponent LightComponents.h "Core/ecs/Components/LightComponents.h"
	 * @brief component with FPointLight member, that can be pushed to FPointLightBatch.
	 */
	struct PointLightComponent {
		
		PointLightComponent() = default;
		PointLightComponent(const PointLightComponent& li) = default;

		FPointLight pointLight;

	};


}


#endif // !MAR_ENGINE_LIGHT_COMPONENTS_H
