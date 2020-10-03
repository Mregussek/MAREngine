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


#include "RenderPipeline.h"
#include "RenderCamera.h"
#include "../GraphicsLogs.h"
#include "../Mesh/ShapeManipulator.h"


namespace mar {
	namespace graphics {


		RenderPipeline RenderPipeline::s_instance;

		void RenderPipeline::initialize() {
			m_availableContainer = &m_containers.emplace_back();

			GRAPHICS_INFO("RENDER_PIPELINE: initialized!");
		}

		void RenderPipeline::reset() {
			for (auto& container : m_containers) {
				container.reset();
			}

			m_containers.clear();

			initialize();

			GRAPHICS_INFO("RENDER_PIPELINE: all data was resetted!");
		}

		void RenderPipeline::setAvailableContainerRenderable(size_t vert_to_push, size_t ind_to_push) {
			size_t current_vertices_size = m_availableContainer->getVertices().size();
			size_t current_indices_size = m_availableContainer->getIndices().size();
			size_t current_transforms_size = m_availableContainer->getTransforms().size();

			if (current_vertices_size + vert_to_push >= settings::maxVerticesCount) {
				m_availableContainer = &m_containers.emplace_back();
			}
			else if (current_indices_size + ind_to_push >= settings::maxIndicesCount) {
				m_availableContainer = &m_containers.emplace_back();
			}
			else if (current_transforms_size + 1 >= 32) {
				m_availableContainer = &m_containers.emplace_back();
			}
		}

		void RenderPipeline::submitRenderable(ecs::RenderableComponent& ren_comp, ecs::TransformComponent& tran) {
			GRAPHICS_INFO("RENDER_PIPELINE: submitting renderable component");

			ShapeManipulator::extendShapeID(ren_comp.vertices, m_availableContainer->m_stride, m_availableContainer->m_shapeID);
			m_availableContainer->m_vertices.insert(m_availableContainer->m_vertices.end(), ren_comp.vertices.begin(), ren_comp.vertices.end());

			uint32_t start_extension = m_availableContainer->m_indices.size();
			uint32_t end_extension = m_availableContainer->m_indices.size() + ren_comp.indices.size();
			m_availableContainer->m_indices.insert(m_availableContainer->m_indices.end(), ren_comp.indices.begin(), ren_comp.indices.end());
			ShapeManipulator::extendIndices(m_availableContainer->m_indices, start_extension, end_extension, m_availableContainer->m_indicesMax);

			ren_comp.shader_id = m_availableContainer->m_shapeID;

			m_availableContainer->m_indicesMax += ren_comp.vertices.size() / m_availableContainer->m_stride;
			m_availableContainer->m_shapeID++;

			m_availableContainer->m_transforms.push_back(tran.transform);

			GRAPHICS_INFO("RENDER_PIPELINE: submitted renderable component {} --- vert_size = {} indi_size = {}, indicesMax = {}, shapeID = {}", 
				ren_comp.id, m_availableContainer->m_vertices.size(), m_availableContainer->m_indices.size(), m_availableContainer->m_indicesMax, m_availableContainer->m_shapeID);
		}

		void RenderPipeline::submitColor(float entity_index, ecs::ColorComponent& color) {
			m_availableContainer->m_colors.push_back({ entity_index, color.texture });
			m_availableContainer->m_samplerTypes.push_back(0.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted color component, current size = {}, entity_index = {}",
				m_availableContainer->m_colors.size(), entity_index);
		}

		void RenderPipeline::submitTexture2D(float entity_index, ecs::Texture2DComponent& tex) {
			m_availableContainer->m_tex2D.push_back({ entity_index, tex.texture });
			m_availableContainer->m_samplerTypes.push_back(1.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture2d component, current size = {}, entity_index = {}, texture2D = {}", 
				m_availableContainer->m_tex2D.size(), entity_index, tex.texture);
		}

		void RenderPipeline::submitCubemap(float entity_index, ecs::TextureCubemapComponent& cube) {
			m_availableContainer->m_cubes.push_back({ entity_index, cube.texture });
			m_availableContainer->m_samplerTypes.push_back(2.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture cubemap component, current size = {}, entity_index = {}, textureCubemap = {}", 
				m_availableContainer->m_cubes.size(), entity_index, cube.texture);
		}

		void RenderPipeline::submitLight(maths::vec3& position, ecs::LightComponent& light) {
			m_availableContainer->m_lights.push_back({ position, light });

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center, current size = {}", m_availableContainer->m_lights.size());
		}

		void RenderPipeline::submitCamera(RenderCamera* cam) {
			m_camera = cam;
		}

		void RenderPipeline::modifyTransform(ecs::TransformComponent& tran, int32_t index) {
			m_availableContainer->m_transforms[index] = tran;

			GRAPHICS_TRACE("RENDER_PIPELINE: modified transform component at index {}", index);
		}

		void RenderPipeline::modifyLight(maths::vec3& position, ecs::LightComponent& light, int32_t index) {
			m_availableContainer->m_lights[index].first = position;
			m_availableContainer->m_lights[index].second = light;

			GRAPHICS_TRACE("RENDER_PIPELINE: modified light component at index {}", index);
		}

		void RenderPipeline::modifyColor(ecs::ColorComponent& color, int32_t index) {
			m_availableContainer->m_colors[index].second = color.texture;

			GRAPHICS_TRACE("RENDER_PIPELINE: modified color component at index {}", index);
		}


} }