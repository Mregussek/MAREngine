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
#include "../../ecs/Components/Components.h"


namespace mar {
	namespace graphics {

		struct RenderCamera;


		class RenderPipeline {
		public:

			RenderPipeline() = default;

			void submitRenderable(ecs::RenderableComponent& ren_comp);
			void submitTransform(ecs::TransformComponent& tran);
			void submitColor(float entity_index, ecs::ColorComponent& color);
			void submitTexture2D(float entity_index, ecs::Texture2DComponent& tex);
			void submitCubemap(float entity_index, ecs::TextureCubemapComponent& cube);
			void submitLight(maths::vec3& position, ecs::LightComponent& light);
			void submitCamera(RenderCamera* cam);

			void modifyTransform(ecs::TransformComponent& tran, int32_t index);
			void modifyLight(maths::vec3& position, ecs::LightComponent& light, int32_t index);
			void modifyColor(ecs::ColorComponent& color, int32_t index);

			void reset();

			float getCurrentShapeID() { return m_shapeID; }

			RenderStatistics& getStatistics() { return s_statistics; }
			void clearStatistics() { s_statistics.resetStatistics(); }

			static RenderPipeline& getInstance() { return s_instance; }

			const std::vector<std::pair<maths::vec3, ecs::LightComponent>>& getLights() const { return m_lights; }
			const std::vector<std::pair<float, maths::vec3>>& getColors() const { return m_colors; }
			const std::vector<std::pair<float, std::string>>& getTextures2D() const { return m_tex2D; }
			const std::vector<std::pair<float, std::string>>& getTexturesCubemap() const { return m_cubes; }
			const std::vector<float> getSamplerTypes() const { return m_samplerTypes; }
			const RenderCamera* getCamera() const { return m_camera; }

			const std::vector<float> getVertices() const { return m_vertices; }
			const std::vector<uint32_t> getIndices() const { return m_indices; }
			size_t getIndicesCount() { return m_indices.size(); }
			const std::vector<maths::mat4> getTransforms() const { return m_transforms; }

		private:

			static RenderPipeline s_instance;

			RenderStatistics s_statistics;

			std::vector<float> m_vertices;
			std::vector<uint32_t> m_indices;
			float m_shapeID{ 0.f };
			uint32_t m_indicesMax{ 0 };
			static const uint32_t m_stride{ 3 + 3 + 2 + 1 };

			std::vector<maths::mat4> m_transforms;

			std::vector<std::pair<float, maths::vec3>> m_colors;
			std::vector<std::pair<float, std::string>> m_tex2D;
			std::vector<std::pair<float, std::string>> m_cubes;
			std::vector<float> m_samplerTypes;

			std::vector<std::pair<maths::vec3, ecs::LightComponent>> m_lights;

			RenderCamera* m_camera{ nullptr };

		};


	}
}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H

