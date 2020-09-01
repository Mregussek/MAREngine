/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H
#define MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H


#include "../../../mar.h"
#include "Renderer.h"
#include "../../ecs/ECS/Components.h"


namespace mar {
	namespace graphics {

		class RendererBatch;



		class RenderPipeline {
			friend class RendererBatch;

			static RenderPipeline s_instance;

			RendererStatistics s_statistics;

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

			RenderCamera* m_camera;

		public:

			RenderPipeline() = default;

			void submitRenderable(ecs::RenderableComponent& ren_comp);
			void submitTransform(ecs::TransformComponent& tran);
			void submitColor(float entity_index, ecs::ColorComponent& color);
			void submitTexture2D(float entity_index, ecs::Texture2DComponent& tex);
			void submitCubemap(float entity_index, ecs::TextureCubemapComponent& cube);
			void submitLight(maths::vec3& position, ecs::LightComponent& light);
			void submitCamera(RenderCamera* cam);

			void reset();

			float getCurrentShapeID() { return m_shapeID; }

			RendererStatistics& getStatistics() { return s_statistics; }
			void clearStatistics() { s_statistics.resetStatistics(); }

			static RenderPipeline& getInstance() { return s_instance; }
		};


	}
}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H

