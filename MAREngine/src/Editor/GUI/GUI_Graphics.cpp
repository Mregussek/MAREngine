/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI_Graphics.h"
#include "../../Platform/OpenGL/ShaderUniforms.h"
#include "../Camera/Camera.h"


namespace mar {
	namespace editor {


		GUI_Graphics GUI_Graphics::s_instance;


		void GUI_Graphics::initialize() {
			uint32_t max_vertex_count = 500000;
			uint32_t max_index_count = 500000 / 2;

			m_pipeline.initialize(max_vertex_count, max_index_count);
			m_pipeline.processLayout();

			m_shader.initialize("resources/shaders/lineloops.shader.glsl");
		}

		void GUI_Graphics::close() { 

			m_pipeline.close();
			m_shader.shutdown();
		}


		void GUI_Graphics::drawSelectedEntity(ecs::RenderableComponent& ren, ecs::TransformComponent& tran) {
			if (!m_canDraw)
				return;

			m_shader.setUniformMat4f("u_MVP", Camera::getCameraData().mvp);
			m_shader.setUniformMat4f(platforms::ShaderUniforms::u_SeparateTransform[(size_t)ren.shader_id], tran.transform);

			m_pipeline.bind();
			m_pipeline.updateBuffers(ren.vertices, ren.indices);

			platforms::DrawingOpenGL::drawLineLoops(ren.indices.size());

			m_pipeline.resetBuffers();
			m_pipeline.unbind();
		}

		void GUI_Graphics::passToDrawEntity(ecs::Entity* e, bool ability_to_draw) {
			if (e) {
				editor::GUI_Graphics::getInstance().setDrawingAbility(ability_to_draw);
				editor::GUI_Graphics::getInstance().drawSelectedEntity(e->getComponent<ecs::RenderableComponent>(), e->getComponent<ecs::TransformComponent>());
			}
			else {
				editor::GUI_Graphics::getInstance().setDrawingAbility(false);
			}
		}


} }
