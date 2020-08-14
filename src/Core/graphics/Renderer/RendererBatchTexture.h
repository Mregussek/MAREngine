/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_BATCH_TEXTURE_H
#define MAR_ENGINE_RENDERER_BATCH_TEXTURE_H

#include "../../../mar.h"
#include "../../ecs/ECS/Entity.h"
#include "../../ecs/ECS/Components.h"
#include "../../ecs/Scene.h"
#include "../../ecs/SceneManager.h"

#include "../../../Editor/Camera/Camera.h"

#include "Shader/ShaderOpenGL.h"
#include "Texture/TextureOpenGL.h"
#include "Buffers/ElementBuffer/ElementBufferOpenGL.h"
#include "Buffers/VertexArray/VertexArrayOpenGL.h"
#include "Buffers/VertexBuffer/VertexBufferOpenGL.h"
#include "Buffers/VertexBuffer/VertexBufferLayoutOpenGL.h"


namespace mar {
	namespace graphics {


		class RendererBatchTexture {
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			ShaderOpenGL m_shader;
			TextureOpenGL m_texture;
			size_t m_lastVerticesSize;

		public:
			void initialize(ShaderType st) {
				const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

				for (size_t i = 0; i < layout.size(); i++)
					m_layout.push(layout[i], PushBuffer::PUSH_FLOAT);

				m_ebo.initialize(constants::maxIndexCount);
				m_vao.initialize();
				m_vbo.initialize(constants::maxVertexCount);
				m_vao.addBuffer(m_layout);

				m_shader.initialize(st);

				m_lastVerticesSize = 0;

				GRAPHICS_INFO("RENDERERENTITY_BATCH_TEXTURE: initialized!");
			}

			void close() {
				m_vao.close();
				m_vbo.close();
				m_ebo.close();

				m_shader.shutdown();
				m_texture.shutdown();

				GRAPHICS_INFO("RENDERERENTITY_BATCH_TEXTURE: closed!");
			}

			void draw() {

			}
		};


} }







#endif // !MAR_ENGINE_RENDERER_BATCH_TEXTURE_H