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


#include "RenderEvents.h"
#include "../graphics/RenderAPI/RenderCamera.h"
#include "../graphics/RenderAPI/RenderPipeline.h"
#include "../graphics/RenderAPI/RenderStatistics.h"
#include "../graphics/GraphicsLogs.h"
#include "../graphics/GraphicLimits.h"
#include "../graphics/Renderer/RenderMemorizer.h"
#include "../graphics/Renderer/PipelineStorage.h"
#include "../graphics/Renderer/ShaderBufferStorage.h"
#include "../ecs/Components/Components.h"
#include "../../Platform/GLSL/ShaderUniforms.h"


namespace marengine {


	RenderEvents RenderEvents::s_instance;

	const RenderEvents& RenderEvents::Instance() { 
		return s_instance; 
	}

	void RenderEvents::onDrawCall() const {
		RenderStatistics::Instance->drawCallsCount += 1;
	
		GRAPHICS_TRACE("RENDER_EVENTS: Handling draw call");
	}

	void RenderEvents::onMainCameraUpdate(const RenderCamera& camera) const {
		passCameraToSSBO(camera);
	}

	void RenderEvents::passCameraToSSBO(const RenderCamera& camera) const {
		GRAPHICS_INFO("RENDER_EVENTS: passing camera data to ssbo!");

		auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };

		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSL_SSBOs::ut_u_MVP.offset, sizeof(mat4), mat4::value_ptr(camera.getMVP()));
	}


}
