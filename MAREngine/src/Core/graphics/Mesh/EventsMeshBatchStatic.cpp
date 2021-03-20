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


#include "EventsMeshBatchStatic.h"
#include "MeshBatchStatic.h"
#include "../RenderAPI/RenderBufferManager.h"
#include "../RenderAPI/RenderPipeline.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	void FEventsMeshBatchStatic::onTransformUpdate(const Entity& entity) {
		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };

		FMeshBatchStatic* batch = [&meshBatchInfoComponent]()->FMeshBatchStatic* {
			if (meshBatchInfoComponent.batchType == EMeshBatchStaticType::COLOR) {
				return &RenderPipeline::Instance->m_staticColorBatches[meshBatchInfoComponent.batchIndex];
			}
			else if (meshBatchInfoComponent.batchType == EMeshBatchStaticType::TEXTURE2D) {
				return &RenderPipeline::Instance->m_staticTexture2DBatches[meshBatchInfoComponent.batchIndex];
			}
			else {
				return nullptr;
			}
		}();

		if (batch) {
			batch->p_transforms[meshBatchInfoComponent.indexAtBatch] = transformComponent.getTransform();
			FRenderBufferManager::onTransformsUpdate(*batch);
		}
	}

	void FEventsMeshBatchStatic::onColorUpdate(const Entity& entity) {
		const auto& colorComponent{ entity.getComponent<ColorComponent>() };
		const auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };

		if (meshBatchInfoComponent.batchType == EMeshBatchStaticType::COLOR) {
			auto& batch{ RenderPipeline::Instance->m_staticColorBatches[meshBatchInfoComponent.batchIndex] };
			batch.m_colors[meshBatchInfoComponent.indexAtBatch] = colorComponent.color;
			FRenderBufferManager::onColorUpdate(batch);
		}
	}


}
