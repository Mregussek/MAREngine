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


#include "MeshBatchStaticColor.h"
#include "../GraphicsLogs.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {



	void FMeshBatchStaticColor::reset() {
		FMeshBatchStatic::reset();

		m_colors.clear();

		m_colorsSSBOindex = 0;
	}

	bool FMeshBatchStaticColor::canBeBatched(const Entity& entity) const {
		if(entity.hasComponent<ColorComponent>() && FMeshBatchStatic::canBeBatched(entity)) {
			return true;
		}
		else {
			return false;
		}
	}

	void FMeshBatchStaticColor::submitToBatch(const Entity& entity) {
		GRAPHICS_TRACE("F_MESH_BATCH_STATIC_COLOR: submitting {} entity to batching...", entity.getComponent<TagComponent>().tag);

		FMeshBatchStatic::submitToBatch(entity);

		const auto& colorComponent{ entity.getComponent<ColorComponent>() };
		submitColorComponent(colorComponent);

		auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };
		meshBatchComponent.materialIndex = m_colors.size() - 1;

		GRAPHICS_INFO("F_MESH_BATCH_STATIC_COLOR: submitted to batch entity {}, meshBatchComponent.materialIndex = {}",
			entity.getComponent<TagComponent>().tag, meshBatchComponent.materialIndex);
	}

	void FMeshBatchStaticColor::submitColorComponent(const ColorComponent& colorComponent) {
		GRAPHICS_TRACE("F_MESH_BATCH_STATIC_COLOR: submitting color component...");

		m_colors.emplace_back(colorComponent.texture);

		GRAPHICS_INFO("F_MESH_BATCH_STATIC_COLOR: submitted color component, current colors size = {}", m_colors.size());
	}

	const FColorsArray& FMeshBatchStaticColor::getColors() const {
		return m_colors;
	}

	uint32_t FMeshBatchStaticColor::getColorsSSBOindex() const {
		return m_colorsSSBOindex;
	}

	void FMeshBatchStaticColor::setColorsSSBOindex(uint32_t index) {
		m_colorsSSBOindex = index;
	}


}
