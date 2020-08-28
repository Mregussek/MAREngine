/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_ENTITY_COLOR_H
#define MAR_ENGINE_RENDERER_ENTITY_COLOR_H


#include "../../../mar.h"
#include "../../ecs/ECS/Entity.h"
#include "../../ecs/ECS/Components.h"
#include "../../ecs/Scene.h"
#include "../../ecs/SceneManager.h"

#include "Shader/ShaderOpenGL.h"
#include "Buffers/ElementBuffer/ElementBufferOpenGL.h"
#include "Buffers/VertexArray/VertexArrayOpenGL.h"
#include "Buffers/VertexBuffer/VertexBufferOpenGL.h"
#include "Buffers/VertexBuffer/VertexBufferLayoutOpenGL.h"



namespace mar {
	namespace graphics {


		class RendererColor {
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			ShaderOpenGL m_shader;

		public:
			void initialize();

			void close();

			void draw(const ecs::BufferStorage<maths::vec3>& storage, 
				const RenderCamera& camera, const ecs::LightStorage& light);

			void passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam);

			void passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light);

		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_COLOR_H