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


#include "RenderStatistics.h"
#include "RenderManager.h"
#include "../../ecs/SceneManager.h"
#include "../../ecs/Scene.h"


namespace marengine {


	RenderStatistics* RenderStatistics::Instance{ nullptr };

	void RenderStatistics::update() {
		verticesCount = {
			FRenderManager::Instance->getStaticColorBatch().getVertices().size() + 
			FRenderManager::Instance->getStaticTexture2DBatch().getVertices().size()
		};
		indicesCount = {
			FRenderManager::Instance->getStaticColorBatch().getIndices().size() +
			FRenderManager::Instance->getStaticTexture2DBatch().getIndices().size()
		};
		trianglesCount = {
			indicesCount / 3
		};
		coloredEntitiesCount = {
			FRenderManager::Instance->getStaticColorBatch().getTransforms().size()
		};
		textured2dEntitiesCount = {
			FRenderManager::Instance->getStaticTexture2DBatch().getTransforms().size()
		};
		allRendererEntitiesCount = {
			coloredEntitiesCount + textured2dEntitiesCount
		};
		entitiesCount = {
			SceneManager::Instance->getScene()->getEntities().size()
		};
	}

	void RenderStatistics::reset() {
		drawCallsCount = 0;
		verticesCount = 0;
		indicesCount = 0;
		trianglesCount = 0;
		entitiesCount = 0;
		coloredEntitiesCount = 0;
		textured2dEntitiesCount = 0;
		allRendererEntitiesCount = 0;
	}


}
