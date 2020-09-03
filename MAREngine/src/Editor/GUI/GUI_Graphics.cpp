/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI_Graphics.h"
#include "../../Platform/OpenGL/ShaderUniforms.h"
#include "../Camera/Camera.h"
#include "../../Core/ecs/ECS/Systems.h"


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
			static size_t index;
			static maths::vec3 scale;
			index = (size_t)ren.shader_id;
			scale = tran.scale + 0.05f;
			maths::mat4 better_outline = ecs::System::handleTransformComponent(tran.center, tran.angles, scale);

			m_pipeline.bind();
			m_pipeline.updateBuffers(ren.vertices, ren.indices);

			m_shader.bind();
			m_shader.setUniformMat4f("u_MVP", Camera::getCameraData().mvp);

			m_pipeline.bind();
			m_pipeline.updateBuffers(ren.vertices, ren.indices);

			//m_shader.setUniformMat4f(platforms::ShaderUniforms::u_SeparateTransform[index], tran.transform);
			//platforms::DrawingOpenGL::drawToStencil(ren.indices.size());

			m_shader.setUniformMat4f(platforms::ShaderUniforms::u_SeparateTransform[index], better_outline);
			platforms::DrawingOpenGL::drawOutline(ren.indices.size());

			m_pipeline.resetBuffers();
			m_pipeline.unbind();
		}

		void GUI_Graphics::passToDrawEntity(ecs::Entity* e, bool ability_to_draw) {
			if (e && ability_to_draw) {

				if (e->hasComponent<ecs::RenderableComponent>())
					editor::GUI_Graphics::getInstance().drawSelectedEntity(e->getComponent<ecs::RenderableComponent>(), e->getComponent<ecs::TransformComponent>());
				
			}
		}


} }
