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


#include "../../../mar.h"


namespace mar::ecs { 
	class Entity; 
	struct RenderPipelineComponent; 
	struct LightComponent;
}
namespace mar::graphics {

	class RenderPipeline;


	class RenderEvents {
	public:

		static RenderEvents& Instance() { return s_instance; }

		void setRenderPipeline(RenderPipeline& renderPipeline);

		void onDrawCall();

		void onTransformMat4Update(const maths::mat4& transfrom, const ecs::RenderPipelineComponent& rpc);
		void onColorUpdate(maths::vec3 color, const ecs::RenderPipelineComponent& rpc);

		void onLightUpdate(maths::vec3 position, const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc);
		void onLightPositionUpdate(maths::vec3 position, const ecs::RenderPipelineComponent& rpc);
		void onLightComponentUpdate(const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc);

	private:

		static RenderEvents s_instance;

		RenderPipeline* m_renderPipeline{ nullptr };

	};


}


#endif // !MAR_ENGINE_GRAPHICS_RENDER_EVENTS_H
