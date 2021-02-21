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
#include "../../graphics/Mesh/MeshDefinitions.h"
#include "../../graphics/Lightning/LightDefinitions.h"


namespace marengine {


	struct MeshBatchInfoComponent {

		int32_t beginVertices{ -1 };
		int32_t endVertices{ -1 };

		int32_t beginIndices{ -1 };
		int32_t endIndices{ -1 };

		int32_t indexAtBatch{ -1 };

		int32_t batchIndex{ -1 };
		EMeshBatchStaticType batchType{ EMeshBatchStaticType::NONE };

	};

	
	struct LightBatchInfoComponent {

		int32_t indexAtBatch{ -1 };

		ELightBatchType batchType{ ELightBatchType::NONE };

	};


}


#endif // !MAR_ENGINE_ECS_ENGINE_ONLY_COMPONENTS_H
