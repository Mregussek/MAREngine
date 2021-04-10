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
#include "../../graphics/Mesh/IMeshBatch.h"
#include "../../graphics/Lightning/LightDefinitions.h"


namespace marengine {


	/**
	 * @struct MeshBatchInfoComponent EngineOnlyComponents.h "Core/ecs/Components/EngineOnlyComponents.h"
	 * @brief Structure used as memorizer for information about mesh batching. We want to remember, at which mesh
	 * batch current entity is stored, where its vertices / indices are beginning & ending and so on.
	 * Used as optimization for RenderAPI.
	 */
	struct MeshBatchInfoComponent {

		int32_t beginVertices{ -1 };
		int32_t endVertices{ -1 };

		int32_t beginIndices{ -1 };
		int32_t endIndices{ -1 };

		int8 indexAtBatch{ -1 };

		int8 batchIndex{ -1 };
		EBatchType batchType{ EBatchType::NONE };

	};

	
	/**
	 * @struct LightBatchInfoComponent EngineOnlyComponents.h "Core/ecs/Components/EngineOnlyComponents.h"
	 * @brief Structure used as memorizer for information about light batching. We want to remember, at which light
	 * batch current entity is stored. Used as optimization for RenderAPI.
	 */
	struct LightBatchInfoComponent {

		int32_t indexAtBatch{ -1 };

		ELightBatchType batchType{ ELightBatchType::NONE };

	};


	// Just declarations for PlayModeStorageComponent ComponentVariant
	struct TransformComponent;
	struct RenderableComponent;
	struct PointLightComponent;

	
	/**
	 * @struct PlayModeStorageComponent EngineOnlyComponents.h "Core/ecs/Components/EngineOnlyComponents.h"
	 * @brief Structure used as storage for components, that can change during play mode. While using 
	 * editor of MAREngine, we want to try game in real-life and then come back to editor state. Component
	 * should be used as storage for play mode components during real-time check with MAREngine's editor.
	 */
	struct PlayModeStorageComponent {

		typedef std::variant<TransformComponent,
		                     RenderableComponent,
		                     PointLightComponent> ComponentVariant;

		enum class ComponentType {
			NONE, TRANSFORM, RENDERABLE, POINTLIGHT
		};

		std::unordered_map<ComponentType, ComponentVariant> components;

	};


}


#endif // !MAR_ENGINE_ECS_ENGINE_ONLY_COMPONENTS_H
