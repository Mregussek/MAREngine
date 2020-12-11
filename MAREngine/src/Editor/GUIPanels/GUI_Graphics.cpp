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
#include "../../Core/graphics/GraphicLimits.h"

#include "../../Core/ecs/Components/Components.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"

#include "../Camera/Camera.h"


namespace mar::editor {


	void GUI_Graphics::initialize() {
		typedef graphics::GraphicLimits marLimits;

		const char* vert = "resources/shaders/lineloops.vert.glsl";
		const char* frag = "resources/shaders/lineloops.frag.glsl";
		const auto shaderPaths = platforms::ShaderPaths(vert, frag, nullptr);

		m_pipeline.initialize(marLimits::sizeOfVertices, marLimits::sizeOfIndices);
		m_shader.initialize(shaderPaths);
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
		using namespace platforms::ShaderUniforms;
		
		const auto outlineScale{ tran.scale + 0.05f };
		const ecs::TransformComponent tranCopy{ tran.center, tran.angles, outlineScale };
		const maths::mat4 betterOutline = tranCopy.getTransform();

		m_pipeline.bind();
		m_pipeline.update(ren.vertices, ren.indices);

		m_shader.bind();
		//m_shader.setUniformMat4(u_MVP, Camera::getCameraData().getMVP());
		//m_shader.setUniformMat4(u_SeparateTransform, betterOutline);

		platforms::DrawingOpenGL::drawOutline(ren.indices.size());

		m_pipeline.reset();
		m_pipeline.unbind();
	}


}
