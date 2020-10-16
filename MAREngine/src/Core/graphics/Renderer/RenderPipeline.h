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
#include "RenderStatistics.h"
#include "RenderContainer.h"
#include "../../ecs/Components/Components.h"


namespace mar::ecs { 
	class Entity; 
}

namespace mar::graphics {

	class RenderCamera;


	class RenderPipeline {
	public:

		RenderPipeline() = default;

		void initialize();
		void reset();

		void submitEntity(const ecs::Entity& entity);

		void submitCamera(RenderCamera* cam);

		void modifyTransform(const ecs::TransformComponent& tran, size_t container_index, size_t transform_index);
		void modifyLight(const maths::vec3& position, const ecs::LightComponent& light, size_t container_index, size_t light_index);
		void modifyColor(const ecs::ColorComponent& color, size_t container_index, size_t color_index);

		RenderStatistics& getStatistics() { return m_statistics; }
		void clearStatistics() { m_statistics.resetStatistics(); }

		static RenderPipeline& Instance() { return *s_instance; }

		// ---- GETTERS for container ---- //
	
		const std::vector<RenderContainer>& getContainers() const { return m_containers; }
		const RenderCamera* getCamera() const { return m_camera; }
		const size_t getAvailableContainerIndex() const { return m_availableContainerIndex; }

		// --- SETTERS --- //

		void setCurrentPipeline() { s_instance = this; }

	private:

		void setAvailableContainerRenderable(ecs::RenderPipelineComponent& rpc, size_t vert_to_push, size_t ind_to_push);
		void setAvailableContainerLight(ecs::RenderPipelineComponent& rpc);

		size_t submitRenderable(ecs::RenderableComponent& ren_comp, const ecs::TransformComponent& tran);
		size_t submitColor(float entity_index, const ecs::ColorComponent& color);
		size_t submitTexture2D(float entity_index, const ecs::Texture2DComponent& tex);
		size_t submitCubemap(float entity_index, const ecs::TextureCubemapComponent& cube);
		size_t submitLight(const maths::vec3& position, const ecs::LightComponent& light);

		static RenderPipeline* s_instance;

		RenderStatistics m_statistics;
		std::vector<RenderContainer> m_containers;
		size_t m_availableContainerIndex{ 0 };

		RenderCamera* m_camera{ nullptr };

	};


}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H

