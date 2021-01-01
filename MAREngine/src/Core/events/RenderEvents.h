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


#ifndef MAR_ENGINE_GRAPHICS_RENDER_EVENTS_H
#define MAR_ENGINE_GRAPHICS_RENDER_EVENTS_H


#include "../../mar.h"


namespace marengine {

	struct RenderPipelineComponent;  struct LightComponent; struct TransformComponent;
	struct GLSL_SSBOs;
	class RenderCamera;


	class RenderEvents {

		typedef maths::vec3 vec3;
		typedef maths::vec4 vec4;
		typedef maths::mat4 mat4;

	public:

		static const RenderEvents& Instance();

		void onDrawCall() const;

		void onContainersReadyToDraw() const;

		void onTransformMat4Update(const TransformComponent& transform, const RenderPipelineComponent& rpc) const;
		void onColorUpdate(vec4 color, const RenderPipelineComponent& rpc) const;
		void onTex2DUpdate(const std::string& texture, const RenderPipelineComponent& rpc) const;

		void onLightUpdate(vec3 position, const LightComponent& light, const RenderPipelineComponent& rpc) const;
		void onLightPositionUpdate(vec3 position, const RenderPipelineComponent& rpc) const;
		void onLightComponentUpdate(const LightComponent& light, const RenderPipelineComponent& rpc) const;
		
		void onMainCameraUpdate(const RenderCamera& camera) const;

	private:

		void passCameraToSSBO(const RenderCamera& camera) const;


		static RenderEvents s_instance;

	};


}


#endif // !MAR_ENGINE_GRAPHICS_RENDER_EVENTS_H
