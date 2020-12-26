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


#include "RenderPipelineHelper.h"
#include "RenderContainer.h"
#include "../GraphicLimits.h"


namespace mar::graphics {


	bool RenderPipelineHelper::canPushRenderableToContainer(const RenderContainer& container, uint32_t verticesToPush, uint32_t indicesToPush) {
		const auto currentVerticesSize{ container.getVertices().size() };
		const auto currentIndicesSize{ container.getIndices().size() };
		const auto currentTransformSize{ container.getTransforms().size() };

		const bool cannotPushVertices = (currentVerticesSize + verticesToPush) >= GraphicLimits::maxVerticesCount;
		const bool cannotPushIndices = (currentIndicesSize + indicesToPush) >= GraphicLimits::maxIndicesCount;
		const bool cannotPushTransform = (currentTransformSize + 1) >= GraphicLimits::maxTransforms;

		return !(cannotPushVertices || cannotPushIndices || cannotPushTransform);
	}

	int32_t RenderPipelineHelper::findAvailableRenderContainer(const std::vector<RenderContainer>& containers, uint32_t verticesToPush, uint32_t indicesToPush) {
		auto containsPlaceForRenderable = [verticesToPush, indicesToPush](const RenderContainer& container) {
			return canPushRenderableToContainer(container, verticesToPush, indicesToPush);
		};

		const auto it = std::find_if(containers.cbegin(), containers.cend(), containsPlaceForRenderable);
		if (it != containers.cend()) {
			const auto index{ (int32_t)std::distance(containers.cbegin(), it) };
			return index;
		}

		return -1;
	}

	int32_t RenderPipelineHelper::findAvailableLightContainer(const std::vector<LightContainer>& containers) {
		auto containsPlaceForLight = [](const LightContainer& container) {
			const auto currentLightSize{ container.getLightMaterials().size() };
			const bool cannotPushLights{ currentLightSize + 1 >= GraphicLimits::maxLights };
			return !cannotPushLights;
		};

		const auto it = std::find_if(containers.cbegin(), containers.cend(), containsPlaceForLight);
		if (it != containers.cend()) {
			const auto index{ (int32_t)std::distance(containers.cbegin(), it) };
			return index;
		}

		return -1;
	}


}
