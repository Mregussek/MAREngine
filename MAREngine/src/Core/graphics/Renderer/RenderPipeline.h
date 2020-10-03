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


namespace mar {
	namespace graphics {

		struct RenderCamera;


		class RenderPipeline {
		public:

			RenderPipeline() = default;

			void initialize();
			void reset();

			void setAvailableContainerRenderable(size_t vert_to_push, size_t ind_to_push);

			void submitRenderable(ecs::RenderableComponent& ren_comp, ecs::TransformComponent& tran);
			void submitColor(float entity_index, ecs::ColorComponent& color);
			void submitTexture2D(float entity_index, ecs::Texture2DComponent& tex);
			void submitCubemap(float entity_index, ecs::TextureCubemapComponent& cube);
			void submitLight(maths::vec3& position, ecs::LightComponent& light);
			void submitCamera(RenderCamera* cam);

			void modifyTransform(ecs::TransformComponent& tran, int32_t index);
			void modifyLight(maths::vec3& position, ecs::LightComponent& light, int32_t index);
			void modifyColor(ecs::ColorComponent& color, int32_t index);

			RenderStatistics& getStatistics() { return s_statistics; }
			void clearStatistics() { s_statistics.resetStatistics(); }

			static RenderPipeline& getInstance() { return s_instance; }

			// ---- GETTERS for container ---- //

			const std::vector<RenderContainer>& getContainers() const { return m_containers; }
			const RenderCamera* getCamera() const { return m_camera; }

		private:

			static RenderPipeline s_instance;

			RenderStatistics s_statistics;
			std::vector<RenderContainer> m_containers;
			RenderContainer* m_availableContainer{ nullptr };

			RenderCamera* m_camera{ nullptr };

		};


	}
}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H

