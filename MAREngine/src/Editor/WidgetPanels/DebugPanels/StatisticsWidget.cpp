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


#include "StatisticsWidget.h"
#include "../../../mar.h"
#include "../../../Debug/Profiler.h"
#include "../../../Core/graphics/RenderAPI/RenderStatistics.h"


namespace marengine {


	void WStatisticsWidget::updateFrame() {
		ImGui::Begin("Statistics Menu");

		const RenderStatistics* stats{ RenderStatistics::Instance };
		
		ImGui::Text("Draw Calls: %d", stats->drawCallsCount);
		ImGui::Text("Vertices: %d", stats->verticesCount);
		ImGui::Text("Indices: %d", stats->indicesCount);
		ImGui::Text("Triangles: %d", stats->trianglesCount);
		ImGui::Text("Entities: %d", stats->entitiesCount);
		ImGui::Text("Colored Entities: %d", stats->coloredEntitiesCount);
		ImGui::Text("Textured2D Entities: %d", stats->textured2dEntitiesCount);
		ImGui::Text("Rendered Entities: %d", stats->allRendererEntitiesCount);

		ImGui::Separator();

		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
		ImGui::Text("ms/frame: %f", 1000.0f / ImGui::GetIO().Framerate);

		ImGui::End();
	}


}

