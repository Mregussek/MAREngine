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


#ifndef MAR_ENGINE_ENGINE_ONLY_COMPONENTS_H
#define MAR_ENGINE_ENGINE_ONLY_COMPONENTS_H


#include "../../../mar.h"


namespace marengine {


	struct RenderPipelineComponent {
		size_t containerIndex{ 0 };
		size_t transformIndex{ 0 };
		size_t colorIndex{ 0 };

		size_t containerLightIndex{ 0 };
		size_t lightIndex{ 0 };

		size_t materialType{ 0 };

		RenderPipelineComponent() = default;
		RenderPipelineComponent(const RenderPipelineComponent& rpc) = default;
	};


}


#endif // !MAR_ENGINE_ECS_ENGINE_ONLY_COMPONENTS_H
