/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_RENDERER_ENTITY_COLOR_H
#define MAR_ENGINE_GRAPHICS_RENDERER_ENTITY_COLOR_H


#include "../../../mar.h"
#include "../../ecs/ECS/Entity.h"
#include "../../ecs/ECS/Components.h"
#include "../../ecs/Scene.h"
#include "../../ecs/SceneManager.h"
#include "Renderer.h"


namespace mar {
	namespace graphics {


		class RendererColor {
			platforms::VertexBufferOpenGL m_vbo;
			platforms::VertexBufferLayoutOpenGL m_layout;
			platforms::VertexArrayOpenGL m_vao;
			platforms::ElementBufferOpenGL m_ebo;
			platforms::ShaderOpenGL m_shader;

		public:
			void initialize();

			void close();

			void draw(const ecs::BufferStorage<maths::vec3>& storage, 
				const RenderCamera& camera, const ecs::LightStorage& light);

			void passCameraToShader(platforms::ShaderOpenGL& shader, const RenderCamera& cam);

			void passLightToShader(platforms::ShaderOpenGL& shader, const ecs::LightStorage& light);

		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_COLOR_H