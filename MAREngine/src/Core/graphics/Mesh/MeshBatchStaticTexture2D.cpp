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


#include "MeshBatchStaticTexture2D.h"
#include "../GraphicsLogs.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	void FMeshBatchStaticTexture2D::reset() {
		FMeshBatchStatic::reset();

		m_textures.clear();
	}

	bool FMeshBatchStaticTexture2D::canBeBatched(const Entity& entity) const {
		if (entity.hasComponent<Texture2DComponent>() && FMeshBatchStatic::canBeBatched(entity)) {
			return true;
		}
		else {
			return false;
		}
	}

	void FMeshBatchStaticTexture2D::submitToBatch(const Entity& entity) {
		GRAPHICS_TRACE("F_MESH_BATCH_STATIC_TEXTURE2D: submitting {} entity to batching...", entity.getComponent<TagComponent>().tag);

		FMeshBatchStatic::submitToBatch(entity);

		const auto& texture2dComponent{ entity.getComponent<Texture2DComponent>() };
		submitTexture2DComponent((uint32_t)(p_meshID - 1.f), texture2dComponent);

		auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };
		meshBatchComponent.materialIndex = m_textures.size() - 1;

		GRAPHICS_TRACE("F_MESH_BATCH_STATIC_TEXTURE2D: submitted to batch entity {}, meshBatchComponent.materialIndex = {}",
			entity.getComponent<TagComponent>().tag, meshBatchComponent.materialIndex);
	}

	void FMeshBatchStaticTexture2D::submitTexture2DComponent(uint32_t bindingIndex, const Texture2DComponent& texture2dComponent) {
		GRAPHICS_TRACE("F_MESH_BATCH_STATIC_TEXTURE2D: submitting texture2D component...");

		m_textures.emplace_back(bindingIndex, texture2dComponent.texture);

		GRAPHICS_TRACE("F_MESH_BATCH_STATIC_TEXTURE2D: submitted texture2D component, current texture2D size = {}", m_textures.size());
	}

	const FTextureArray& FMeshBatchStaticTexture2D::getTextures() const {
		return m_textures;
	}


}
