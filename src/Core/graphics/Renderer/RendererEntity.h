/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_ENTITY_H
#define MAR_ENGINE_RENDERER_ENTITY_H

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

#include "Renderer.h"


namespace mar {
	namespace graphics {

		struct RendererStatistics;


		class RendererEntity {
			// --- BUFFERS --- //
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			ShaderOpenGL m_shaderColor;
			ShaderOpenGL m_shaderTextures2D;
			ShaderOpenGL m_shaderCubemaps;

			// --- OTHER --- //

			static RendererStatistics s_stats;

		public:
			
			void initialize();

			void close();
			
			void update(ecs::Scene* scene);

			// --- STATISTICS --- //
			
			static RendererStatistics& getStatistics();
			static void clearStatistics();

		private:
			// --- DRAW METHODS --- //

			void draw(
				const ecs::SceneStorage<maths::vec3>& storage, 
				const RenderCamera& camera,
				const ecs::LightStorage& light,
				ShaderOpenGL& shader);

			void draw(
				const ecs::SceneStorage<int32_t>& storage,
				const RenderCamera& camera,
				const ecs::LightStorage& light, 
				const TextureOpenGL& texture,
				ShaderOpenGL& shader,
				const int32_t& texture_type
			);
		
			// --- OTHER METHODS / HELPERS --- //

			void passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light);
			void passCameraToShader(ShaderOpenGL& shader, const RenderCamera& cam);
		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_H
