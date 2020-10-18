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


#include "GUI_Graphics.h"

#include "../../Platform/OpenGL/ShaderUniforms.h"
#include "../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../Core/graphics/Renderer/RenderContainer.h"

#include "../../Core/ecs/Components/Components.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"

#include "../Camera/Camera.h"


namespace mar::editor {


	void GUI_Graphics::initialize() {
		m_pipeline.initialize(settings::maxVerticesCount, settings::maxIndicesCount);
		m_shader.initialize("resources/shaders/lineloops.shader.glsl");
	}

	void GUI_Graphics::close() { 
		m_pipeline.close();
		m_shader.shutdown();
	}

	void GUI_Graphics::passToDrawCollection(const ecs::EntityCollection& collection) {

	}

	void GUI_Graphics::passToDrawEntity(const ecs::Entity& entity) {
		if (entity.hasComponent<ecs::RenderableComponent>()) {
			const auto& renderable = entity.getComponent<ecs::RenderableComponent>();
			const auto& transform = entity.getComponent<ecs::TransformComponent>();

			drawSelectedEntity(renderable, transform);
		}		
	}

	void GUI_Graphics::drawSelectedEntity(const ecs::RenderableComponent& ren, const ecs::TransformComponent& tran) {
		const size_t index = 0;
		const maths::vec3 scale = tran.scale + 0.05f;
		const maths::mat4 betterOutline = ecs::TransformComponent::calculate(tran.center, tran.angles, scale);

		m_pipeline.bind();
		m_pipeline.update(ren.vertices, ren.indices);

		m_shader.bind();
		m_shader.setUniformMat4("u_MVP", Camera::getCameraData().getMVP());
		m_shader.setUniformMat4(platforms::ShaderUniforms::u_SeparateTransform[index], betterOutline);

		platforms::DrawingOpenGL::drawOutline(ren.indices.size());

		m_pipeline.reset();
		m_pipeline.unbind();
	}


}
