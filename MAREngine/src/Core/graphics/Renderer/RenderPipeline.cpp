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
#include "../../ecs/Entity/Entity.h"


namespace mar {
	namespace graphics {


		RenderPipeline RenderPipeline::s_instance;

		void RenderPipeline::initialize() {
			m_containers.emplace_back();
			m_availableContainerIndex = m_containers.size() - 1;

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

		void RenderPipeline::pushEntityToPipeline(const ecs::Entity& entity) {
			using namespace ecs;

			auto& tran = entity.getComponent<TransformComponent>();

			if (entity.hasComponent<LightComponent>()) {
				auto& light = entity.getComponent<LightComponent>();
				auto& rpc = entity.getComponent<RenderPipelineComponent>();

				setAvailableContainerLight(rpc);
				rpc.light_index = submitLight(tran.center, light);
			}

			if (entity.hasComponent<RenderableComponent>()) {
				auto& ren = entity.getComponent<RenderableComponent>();
				auto& rpc = entity.getComponent<RenderPipelineComponent>();

				setAvailableContainerRenderable(rpc, ren.vertices.size(), ren.indices.size());
				rpc.transform_index = submitRenderable(ren, tran);

				if (entity.hasComponent<ColorComponent>()) {
					auto& color = entity.getComponent<ColorComponent>();
					rpc.color_index = submitColor(ren.shader_id, color);
				}
				else if (entity.hasComponent<Texture2DComponent>()) {
					auto& tex = entity.getComponent<Texture2DComponent>();
					rpc.color_index = submitTexture2D(ren.shader_id, tex);
				}
				else if (entity.hasComponent<TextureCubemapComponent>()) {
					auto& cube = entity.getComponent<TextureCubemapComponent>();
					rpc.color_index = submitCubemap(ren.shader_id, cube);
				}
			}
		}

		void RenderPipeline::setAvailableContainerRenderable(ecs::RenderPipelineComponent& rpc, size_t vert_to_push, size_t ind_to_push) {
			size_t current_vertices_size;
			size_t current_indices_size;
			size_t current_transforms_size;

			for (size_t i = 0; i < m_containers.size(); i++) {
				current_vertices_size = m_containers[i].getVertices().size();
				current_indices_size = m_containers[i].getIndices().size();
				current_transforms_size = m_containers[i].getTransforms().size();

				if (current_vertices_size + vert_to_push >= settings::maxVerticesCount ||
					current_indices_size + ind_to_push >= settings::maxIndicesCount ||
					current_transforms_size + 1 >= 32)
				{
					continue;
				}
				else {
					m_availableContainerIndex = i;
					rpc.container_index = m_availableContainerIndex;
					GRAPHICS_TRACE("RENDER_PIPELINE: available container is at index {}, returning...", i);
					return;
				}
			}

			m_containers.emplace_back();
			m_availableContainerIndex = m_containers.size() - 1;
			rpc.container_index = m_availableContainerIndex;
			GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container, current size {}", m_containers.size());
		}

		void RenderPipeline::setAvailableContainerLight(ecs::RenderPipelineComponent& rpc) {
			size_t current_light_size;

			for (size_t i = 0; i < m_containers.size(); i++) {
				current_light_size = m_containers[i].getLights().size();

				if (current_light_size + 1 >= 32) { continue; }
				else {
					m_availableContainerIndex = i;
					rpc.container_light_index = m_availableContainerIndex;
					GRAPHICS_TRACE("RENDER_PIPELINE: available container for light is at {}, returning...", i);
					return;
				}
			}
			
			m_containers.emplace_back();
			m_availableContainerIndex = m_containers.size() - 1;
			rpc.container_light_index = m_availableContainerIndex;
			GRAPHICS_INFO("RENDER_PIPELINE: emplaced back new render container (for light), current size {}", m_containers.size());
		}

		size_t RenderPipeline::submitRenderable(ecs::RenderableComponent& ren_comp, const ecs::TransformComponent& tran) {
			GRAPHICS_INFO("RENDER_PIPELINE: submitting renderable component");
			RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];

			ShapeManipulator::extendShapeID(ren_comp.vertices, availableContainer->m_stride, availableContainer->m_shapeID);
			availableContainer->m_vertices.insert(availableContainer->m_vertices.end(), ren_comp.vertices.begin(), ren_comp.vertices.end());

			const uint32_t startExtensionIndices = availableContainer->m_indices.size();
			const uint32_t endExtensionIndices = availableContainer->m_indices.size() + ren_comp.indices.size();
			availableContainer->m_indices.insert(availableContainer->m_indices.end(), ren_comp.indices.begin(), ren_comp.indices.end());
			ShapeManipulator::extendIndices(availableContainer->m_indices, startExtensionIndices, endExtensionIndices, availableContainer->m_indicesMax);

			ren_comp.shader_id = availableContainer->m_shapeID;

			availableContainer->m_indicesMax += ren_comp.vertices.size() / availableContainer->m_stride;
			availableContainer->m_shapeID++;

			availableContainer->m_transforms.push_back(tran.transform);

			GRAPHICS_INFO("RENDER_PIPELINE: submitted renderable component {} --- vert_size = {} indi_size = {}, indicesMax = {}, shapeID = {}", 
				ren_comp.id, availableContainer->m_vertices.size(), availableContainer->m_indices.size(), availableContainer->m_indicesMax, availableContainer->m_shapeID);
			
			return availableContainer->m_transforms.size() - 1;
		}

		size_t RenderPipeline::submitColor(float entity_index, const ecs::ColorComponent& color) {
			RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
			availableContainer->m_colors.push_back({ entity_index, color.texture });
			availableContainer->m_samplerTypes.push_back(0.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted color component, current size = {}, entity_index = {}",
				availableContainer->m_colors.size(), entity_index);
			
			return availableContainer->m_colors.size() - 1;
		}

		size_t RenderPipeline::submitTexture2D(float entity_index, const ecs::Texture2DComponent& tex) {
			RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
			availableContainer->m_tex2D.push_back({ entity_index, tex.texture });
			availableContainer->m_samplerTypes.push_back(1.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture2d component, current size = {}, entity_index = {}, texture2D = {}", 
				availableContainer->m_tex2D.size(), entity_index, tex.texture);

			return availableContainer->m_tex2D.size() - 1;
		}

		size_t RenderPipeline::submitCubemap(float entity_index, const ecs::TextureCubemapComponent& cube) {
			RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
			availableContainer->m_cubes.push_back({ entity_index, cube.texture });
			availableContainer->m_samplerTypes.push_back(2.0f);

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted texture cubemap component, current size = {}, entity_index = {}, textureCubemap = {}", 
				availableContainer->m_cubes.size(), entity_index, cube.texture);

			return availableContainer->m_cubes.size() - 1;
		}

		size_t RenderPipeline::submitLight(const maths::vec3& position, const ecs::LightComponent& light) {
			RenderContainer* availableContainer = &m_containers[m_availableContainerIndex];
			availableContainer->m_lights.push_back({ position, light });

			GRAPHICS_TRACE("RENDER_PIPELINE: submitted light component with its center, current size = {}", 
				availableContainer->m_lights.size());
			
			return availableContainer->m_lights.size() - 1;
		}

		void RenderPipeline::submitCamera(RenderCamera* cam) {
			m_camera = cam;
		}

		void RenderPipeline::modifyTransform(ecs::TransformComponent& tran, size_t container_index, size_t transform_index) {
			m_containers[container_index].m_transforms[transform_index] = tran;

			GRAPHICS_TRACE("RENDER_PIPELINE: modified transform component {} at container {}", transform_index, container_index);
		}

		void RenderPipeline::modifyLight(maths::vec3& position, ecs::LightComponent& light, size_t container_index, size_t light_index) {
			m_containers[container_index].m_lights[light_index].first = position;
			m_containers[container_index].m_lights[light_index].second = light;

			GRAPHICS_TRACE("RENDER_PIPELINE: modified light component {} at container {}", light_index, container_index);
		}

		void RenderPipeline::modifyColor(ecs::ColorComponent& color, size_t container_index, size_t color_index) {
			m_containers[container_index].m_colors[color_index].second = color.texture;

			GRAPHICS_TRACE("RENDER_PIPELINE: modified color component {} at container {}", color_index, container_index);
		}


} }