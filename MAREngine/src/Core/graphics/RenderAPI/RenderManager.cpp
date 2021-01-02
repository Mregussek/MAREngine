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


#include "RenderManager.h"
#include "../GraphicLimits.h"
#include "../Renderer/PipelineStorage.h"
#include "../../ecs/SceneManager.h"
#include "../../ecs/Scene.h"
#include "../../ecs/Entity/Entity.h"


namespace marengine {


	FRenderManager* FRenderManager::Instance{ nullptr };


	void FRenderManager::reset() {
		m_staticColorMeshBatch.reset();
		m_staticTexture2DMeshBatch.reset();
		m_pointLightBatch.reset();
	}

	void FRenderManager::batchEntities(const std::vector<Entity>& entities) {
		for (const Entity& entity : entities) {
			if (m_staticColorMeshBatch.canBeBatched(entity)) {
				m_staticColorMeshBatch.submitToBatch(entity);
			}

			if (m_staticTexture2DMeshBatch.canBeBatched(entity)) {
				m_staticTexture2DMeshBatch.submitToBatch(entity);
			}

			if (m_pointLightBatch.canBeBatched(entity)) {
				m_pointLightBatch.submitEntityWithLightning(entity);
			}
		}

		onContainersReadyToDraw();
	}

	void FRenderManager::onContainersReadyToDraw() {
		PipelineStorage::Instance->close();

		auto createPipelineStorage = [](FMeshBatchStatic* batch) {
			auto& pipeline = PipelineStorage::Instance->createPipeline();
			pipeline.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);
			pipeline.bind();
			pipeline.update(batch->getVertices(), batch->getIndices());
			batch->setUniquePipelineID(PipelineStorage::Instance->getPipelines().size() - 1);
		};

		createPipelineStorage(&m_staticColorMeshBatch);
		createPipelineStorage(&m_staticTexture2DMeshBatch);
	}

	const FMeshBatchStaticColor& FRenderManager::getStaticColorBatch() const {
		return m_staticColorMeshBatch;
	}

	const FMeshBatchStaticTexture2D& FRenderManager::getStaticTexture2DBatch() const {
		return m_staticTexture2DMeshBatch;
	}

	const FPointLightBatch& FRenderManager::getPointLightsBatch() const {
		return m_pointLightBatch;
	}


}
