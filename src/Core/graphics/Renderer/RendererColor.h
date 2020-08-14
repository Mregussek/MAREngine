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

#include "../../../Editor/Camera/Camera.h"

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
			size_t m_lastVerticesSize;

		public:
			void initialize() {
				const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

				for (size_t i = 0; i < layout.size(); i++)
					m_layout.push(layout[i], PushBuffer::PUSH_FLOAT);

				m_ebo.initialize(constants::maxIndexCount);
				m_vao.initialize();
				m_vbo.initialize(constants::maxVertexCount);
				m_vao.addBuffer(m_layout);

				m_shader.initialize(SHADER_ENTITY_COLOR);

				m_lastVerticesSize = 0;

				GRAPHICS_INFO("RENDERERENTITY_COLOR: initialized!");
			}

			void close() {
				m_vao.close();
				m_vbo.close();
				m_ebo.close();

				m_shader.shutdown();

				GRAPHICS_INFO("RENDERERENTITY_COLOR: closed!");
			}

			void draw(
				const ecs::BufferStorage<maths::vec3>& storage, 
				const RenderCamera& camera,
				const ecs::LightStorage& light
			) {
				GRAPHICS_TRACE("RENDERERENTITY_COLOR: is preparing to draw!");

				{ // SEND ALL DATA TO SHADERS
					m_shader.bind();

					passLightToShader(m_shader, light);
					passCameraToShader(m_shader, camera);

					m_shader.setUniformVectorMat4("u_SeparateTransform", storage.transforms);
					m_shader.setUniformVectorVec3("u_SeparateColor", storage.samplers);
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

				GRAPHICS_INFO("RENDERERENTITY_COLOR: has drawn the scene!");
			}

			void passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light);

			void passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam) {
				shader.setUniformVector3("u_CameraPos", cam.position);
				shader.setUniformMat4f("u_Model", cam.model);
				shader.setUniformMat4f("u_MVP", cam.mvp);

				GRAPHICS_TRACE("RENDERERENTITY_COLOR: passed camera to shader (by values)!");
			}



		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_COLOR_H