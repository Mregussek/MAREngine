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


#ifndef MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H
#define MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H


#include "../../../mar.h"
#include "RenderContainer.h"
#include "../../ecs/Components/Components.h"


namespace mar::ecs {  class Entity; }
namespace mar::graphics {

	class RenderCamera;


	class RenderPipeline {

		friend class RenderEvents;

	public:

		RenderPipeline() = default;

		void initialize();
		void reset();

		void submitEntity(const ecs::Entity& entity);

		void submitCamera(const RenderCamera* cam);

		static RenderPipeline& Instance();

		// ---- GETTERS for container ---- //
	
		const std::vector<RenderContainer>& get2Dcontainers() const;
		const std::vector<RenderContainer>& getCubemapContainers() const;
		const std::vector<LightContainer>& getLightContainers() const;
		const RenderCamera* getCamera() const;

		// --- SETTERS --- //

		void setCurrentPipeline();

	private:

		void setContainerRenderable(MaterialRenderType materialType, ecs::RenderPipelineComponent& rpc, size_t verticesToPush, size_t indicesToPush);
		void setContainerLight(ecs::RenderPipelineComponent& rpc);

		size_t submitRenderable(const ecs::RenderableComponent& renderable, const ecs::TransformComponent& transform);
		size_t submitColor(int32_t entityIndex, const ecs::ColorComponent& color);
		size_t submitTexture2D(int32_t entityIndex, const ecs::Texture2DComponent& texture);
		size_t submitCubemap(int32_t entityIndex, const ecs::TextureCubemapComponent& cubemap);
		size_t submitLight(const maths::vec3& position, const ecs::LightComponent& light);


		static RenderPipeline* s_instance;

		std::vector<RenderContainer> m_containers2D;
		std::vector<RenderContainer> m_containersCubemap;
		std::vector<LightContainer> m_lights;

		RenderContainer* m_containerPtr{ nullptr };
		LightContainer* m_lightPtr{ nullptr };

		const RenderCamera* m_camera{ nullptr };

	};


}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H

