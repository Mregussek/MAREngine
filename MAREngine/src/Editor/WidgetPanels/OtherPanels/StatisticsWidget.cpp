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


#include "StatisticsWidget.h"
#include "../../../mar.h"
#include "../../EditorLogging.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/graphics/RenderAPI/RenderStatistics.h"
#include "../../../Core/graphics/RenderAPI/RenderPipeline.h"


namespace marengine {


	void WStatisticsWidget::updateFrame() {
		ImGui::Begin("Statistics Menu");

		auto& stats{ *RenderStatistics::Instance };
		const auto& containersColor{ RenderPipeline::Instance->getColorContainers() };
		const auto& containers2D{ RenderPipeline::Instance->get2Dcontainers() };
		const auto& containersCubemap{ RenderPipeline::Instance->getCubemapContainers() };

		const auto& entities{ SceneManager::Instance->getScene()->getEntities() };

		stats.entitiesCount += entities.size();
		stats.allEntitiesCount += stats.entitiesCount;

		auto pushContainerDataToStats = [&stats](const std::vector<RenderContainer>& containers) {
			std::for_each(containers.cbegin(), containers.cend(), [&stats](const RenderContainer& container) {
				stats.shapesCount += container.getTransforms().size();
				stats.verticesCount += container.getVertices().size();
				stats.indicesCount += container.getIndices().size();
				stats.trianglesCount += container.getIndices().size() / 3;
			});
		};

		pushContainerDataToStats(containersColor);
		pushContainerDataToStats(containers2D);
		pushContainerDataToStats(containersCubemap);

		auto entityHasRenderable = [](const Entity& entity) {
			return entity.hasComponent<RenderableComponent>();
		};

		uint32_t renderablesEntities = std::count_if(entities.cbegin(), entities.cend(), entityHasRenderable);

		ImGui::Text("Draw Calls: %d", stats.drawCallsCount);
		ImGui::Text("Vertices: %d", stats.verticesCount);
		ImGui::Text("Indices: %d", stats.indicesCount);
		ImGui::Text("Triangles: %d", stats.trianglesCount);
		ImGui::Text("Entities: %d", stats.entitiesCount);
		ImGui::Text("All Entities: %d", stats.allEntitiesCount);
		ImGui::Text("All Entities with Renderable: %d", renderablesEntities);

		ImGui::Separator();

		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
		ImGui::Text("ms/frame: %f", 1000.0f / ImGui::GetIO().Framerate);

		ImGui::End();

		EDITOR_TRACE("GUI: scene_statistics");
	}


}

