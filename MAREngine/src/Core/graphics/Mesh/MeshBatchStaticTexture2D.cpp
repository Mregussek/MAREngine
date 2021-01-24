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


#include "MeshBatchStaticTexture2D.h"
#include "../RenderAPI/RenderPipeline.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	void FMeshBatchStaticTexture2D::reset() {
		FMeshBatchStatic::reset();

		m_textures.clear();
	}

	bool FMeshBatchStaticTexture2D::canBeBatched(const Entity& entity) const {
		const bool baseClassPermission{ FMeshBatchStatic::canBeBatched(entity) };

		if (baseClassPermission && entity.hasComponent<Texture2DComponent>()) {
			return true;
		}
		else {
			return false;
		}
	}

	void FMeshBatchStaticTexture2D::submitToBatch(const Entity& entity) {
		FMeshBatchStatic::submitToBatch(entity);

		const uint32_t bindingIndex{ ((uint32_t)p_shapeID - 1) };
		submitTexture(bindingIndex, entity.getComponent<Texture2DComponent>());

		auto& rpc{ entity.getComponent<RenderPipelineComponent>() };
		rpc.colorIndex = m_textures.size() - 1;
		rpc.materialType = (size_t)s_meshBatchType;
	}

	void FMeshBatchStaticTexture2D::submitTexture(uint32_t bindingIndex, const Texture2DComponent& textureComponent) {
		m_textures.emplace_back(bindingIndex, textureComponent.texturePath);
	}

	const FTexturesArray& FMeshBatchStaticTexture2D::getTextures() const {
		return m_textures;
	}


}
