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
		const auto& renderPipelineComponent{ entity.getComponent<RenderPipelineComponent>() };

		if (renderPipelineComponent.materialType == (size_t)EMeshBatchStaticType::COLOR) {
			auto& batch{ RenderPipeline::Instance->m_staticColorBatches[renderPipelineComponent.containerIndex] };
			batch.p_transforms[renderPipelineComponent.transformIndex] = transformComponent.getTransform();
			FRenderBufferManager::onTransformsUpdate(batch);
		}
		else if (renderPipelineComponent.materialType == (size_t)EMeshBatchStaticType::TEXTURE2D) {
			auto& batch{ RenderPipeline::Instance->m_staticTexture2DBatches[renderPipelineComponent.containerIndex] };
			batch.p_transforms[renderPipelineComponent.transformIndex] = transformComponent.getTransform();
			FRenderBufferManager::onTransformsUpdate(batch);
		}
	}

	void FEventsMeshBatchStatic::onColorUpdate(const Entity& entity) {
		const auto& colorComponent{ entity.getComponent<ColorComponent>() };
		const auto& renderPipelineComponent{ entity.getComponent<RenderPipelineComponent>() };

		if (renderPipelineComponent.materialType == (size_t)EMeshBatchStaticType::COLOR) {
			auto& batch{ RenderPipeline::Instance->m_staticColorBatches[renderPipelineComponent.containerIndex] };
			batch.m_colors[renderPipelineComponent.colorIndex] = colorComponent.color;
			FRenderBufferManager::onColorUpdate(batch);
		}
	}


}
