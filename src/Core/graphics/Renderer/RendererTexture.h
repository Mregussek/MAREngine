/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_ENTITY_TEXTURE_H
#define MAR_ENGINE_RENDERER_ENTITY_TEXTURE_H


#include "../../../mar.h"
#include "../../ecs/ECS/Entity.h"
#include "../../ecs/ECS/Components.h"
#include "../../ecs/Scene.h"
#include "../../ecs/SceneManager.h"
#include "Renderer.h"



namespace mar {
	namespace graphics {


		class RendererTexture {
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			ShaderOpenGL m_shader;
			TextureOpenGL m_texture;
			size_t m_lastVerticesSize;

		public:
			void initialize(ShaderType st);

			void close();

			void draw(
				const ecs::BufferStorage<int32_t>& storage, const RenderCamera& camera,
				const ecs::LightStorage& light, uint32_t texture_type
			);

			void passTexturesToShader(ShaderOpenGL& shader, uint32_t texture_type, 
				const std::vector<std::string>& paths, const std::vector<int32_t>& samplers);

			void passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light);

			void passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam);



		};


	}
}


#endif // !MAR_ENGINE_RENDERER_ENTITY_TEXTURE_H