/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_ENTITY_TEXTURE_H
#define MAR_ENGINE_RENDERER_ENTITY_TEXTURE_H


#include "../../../mar.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Components.h"
#include "../../ecs/Scene.h"

#include "../../../Editor/Camera/Camera.h"

#include "Shader/ShaderOpenGL.h"
#include "Buffers/ElementBuffer/ElementBufferOpenGL.h"
#include "Buffers/VertexArray/VertexArrayOpenGL.h"
#include "Buffers/VertexBuffer/VertexBufferOpenGL.h"
#include "Buffers/VertexBuffer/VertexBufferLayoutOpenGL.h"



namespace mar {
	namespace graphics {


		class RendererTexture {
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			ShaderOpenGL m_shader;
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

				GRAPHICS_INFO("RENDERERENTITY_TEXTURE: initialized!");
			}

			void close() {
				m_vao.close();
				m_vbo.close();
				m_ebo.close();

				m_shader.shutdown();

				GRAPHICS_INFO("RENDERERENTITY_TEXTURE: closed!");
			}

			void draw(
				const ecs::SceneStorage<int32_t>& storage,
				const RenderCamera& camera, 
				const ecs::LightStorage& light, 
				const TextureOpenGL& texture,
				const int32_t& texture_type
			) {
				GRAPHICS_TRACE("RENDERERENTITY_TEXTURE: is preparing to draw!");

				{ // BIND TEXTURES
					if (texture_type == GL_TEXTURE_2D)
						for (int32_t i = 0; i < (int32_t)storage.samplers.size(); i++)
							texture.bind(texture_type, storage.samplers[i], texture.getTexture(storage.paths[i]));
					else if (texture_type == GL_TEXTURE_CUBE_MAP)
						for (int32_t i = 0; i < (int32_t)storage.samplers.size(); i++)
							texture.bind(texture_type, storage.samplers[i], texture.getCubemap(storage.paths[i]));
				}

				{ // SEND ALL DATA TO SHADERS
					m_shader.bind();

					passLightToShader(m_shader, light);
					passCameraToShader(m_shader, camera);

					m_shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
					m_shader.setUniformSampler("u_SeparateColor", storage.samplers);
				}

				{ // BIND ALL NEEDED BUFFERS
					m_vao.bind();
					m_vbo.bind();
					m_ebo.bind();

					if (m_lastVerticesSize != storage.vertices.size()) {
						m_vbo.updateDynamically(storage.vertices);
						m_ebo.update(storage.indices);

						m_lastVerticesSize = storage.vertices.size();
					}
				}

				MAR_CORE_GL_FUNC(glDrawElements(GL_TRIANGLES, storage.indices.size(), GL_UNSIGNED_INT, nullptr));

				GRAPHICS_INFO("RENDERERENTITY_TEXTURE: has drawn the scene!");
			}

			void passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light);

			void passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam) {
				shader.setUniformVector3("u_CameraPos", cam.position);
				shader.setUniformMat4f("u_Model", cam.model);
				shader.setUniformMat4f("u_MVP", cam.mvp);

				GRAPHICS_TRACE("RENDERERENTITY_TEXTURE: passed camera to shader (by values)!");
			}



		};


	}
}


#endif // !MAR_ENGINE_RENDERER_ENTITY_TEXTURE_H