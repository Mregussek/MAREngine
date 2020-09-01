/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "RenderPipeline.h"
#include "../Mesh/ShapeManipulator.h"


namespace mar {
	namespace graphics {


		RenderPipeline RenderPipeline::s_instance;


		void RenderPipeline::submitRenderable(ecs::RenderableComponent& ren_comp) {
			GRAPHICS_INFO("RENDER_PIPELINE: submitting renderable component");

			ShapeManipulator::extendShapeID(ren_comp.vertices, m_stride, m_shapeID);
			m_vertices.insert(m_vertices.end(), ren_comp.vertices.begin(), ren_comp.vertices.end());

			uint32_t start_extension = m_indices.size();
			uint32_t end_extension = m_indices.size() + ren_comp.indices.size();
			m_indices.insert(m_indices.end(), ren_comp.indices.begin(), ren_comp.indices.end());
			ShapeManipulator::extendIndices(m_indices, start_extension, end_extension, m_indicesMax);

			m_indicesMax += ren_comp.vertices.size() / m_stride;
			m_shapeID++;

			GRAPHICS_INFO("RENDER_PIPELINE: submitted renderable component {} --- vert_size = {} indi_size = {}, indicesMax = {}, shapeID = {}", ren_comp.id, m_vertices.size(), m_indices.size(), m_indicesMax, m_shapeID);
		}

		void RenderPipeline::submitTransform(ecs::TransformComponent& tran) {
			m_transforms.push_back(tran.transform);
			
			GRAPHICS_TRACE("RENDER_PIPELINE: submitted transform component, current size = {}", m_transforms.size());
		}

		void RenderPipeline::submitColor(float entity_index, ecs::ColorComponent& color) {
			m_colors.push_back({ entity_index, color.texture });
			m_samplerTypes.push_back(0.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted color component, current size = {}, entity_index = {}", m_colors.size(), entity_index);
		}

		void RenderPipeline::submitTexture2D(float entity_index, ecs::Texture2DComponent& tex) {
			m_tex2D.push_back({ entity_index, tex.texture });
			m_samplerTypes.push_back(1.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture2d component, current size = {}, entity_index = {}, texture2D = {}", m_tex2D.size(), entity_index, tex.texture);
		}

		void RenderPipeline::submitCubemap(float entity_index, ecs::TextureCubemapComponent& cube) {
			m_cubes.push_back({ entity_index, cube.texture });
			m_samplerTypes.push_back(2.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture cubemap component, current size = {}, entity_index = {}, textureCubemap = {}", m_cubes.size(), entity_index, cube.texture);
		}

		void RenderPipeline::submitLight(maths::vec3& position, ecs::LightComponent& light) {
			m_lights.push_back({ position, light });

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center");
		}

		void RenderPipeline::submitCamera(RenderCamera* cam) {
			m_camera = cam;
		}

		void RenderPipeline::reset() {
			m_vertices.clear();
			m_indices.clear();
			m_shapeID = 0.f;
			m_indicesMax = 0;

			m_colors.clear();
			m_tex2D.clear();
			m_cubes.clear();
			m_samplerTypes.clear();

			m_lights.clear();

			GRAPHICS_INFO("RENDER_PIPELINE: all data was resetted!");
		}

} }