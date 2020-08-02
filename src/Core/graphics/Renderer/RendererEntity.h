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
			ShaderOpenGL m_shaderTexture2D;
			ShaderOpenGL m_shaderCubemap;

			// --- STORAGE --- //
			std::vector<float> m_verticesColor;
			std::vector<uint32_t> m_indicesColor;
			std::vector<maths::mat4> m_transformsColor;
			std::vector<maths::vec3> m_samplersColors;
			int32_t m_counterColor;
			uint32_t m_indicesMaxColor;

			std::vector<float> m_verticesTexture2D;
			std::vector<uint32_t> m_indicesTexture2D;
			std::vector<maths::mat4> m_transformsTexture2D;
			std::vector<int32_t> m_samplersTexture2D;
			int32_t m_counterTexture2D;
			uint32_t m_indicesMaxTexture2D;

			std::vector<float> m_verticesCubemap;
			std::vector<uint32_t> m_indicesCubemap;
			std::vector<maths::mat4> m_transformsCubemap;
			std::vector<int32_t> m_samplersCubemap;
			int32_t m_counterCubemap;
			uint32_t m_indicesMaxCubemap;

			// --- OTHER --- //

			std::vector<maths::vec3> m_lightPositions;
			std::vector<ecs::LightComponent> m_lightComponents;

			maths::mat4 m_cameraModel;
			maths::mat4 m_cameraMVP;
			maths::vec3 m_cameraCenter;

			uint32_t m_stride;
			bool m_lastSizeSet;
			uint32_t m_lastSize;
			static RendererStatistics s_stats;

		public:
			
			void initialize();

			void close();

			void submit(ecs::Scene* scene);
			
			void update();

			void clear();

			// --- STATISTICS --- //
			
			static RendererStatistics& getStatistics();
			static void clearStatistics();

		private:
			// --- DRAW METHODS --- //

			void draw(
				const std::vector<float>& vertices, 
				const std::vector<uint32_t>& indices,
				const std::vector<maths::mat4>& transforms, 
				const std::vector<maths::vec3>& samplers, 
				ShaderOpenGL& shader
			);

			void draw(
				const std::vector<float>& vertices, 
				const std::vector<uint32_t>& indices,
				const std::vector<maths::mat4>& transforms, 
				const std::vector<int32_t>& samplers, 
				ShaderOpenGL& shader
			);
		
			// --- OTHER METHODS / HELPERS --- //

			void submitEntity(ecs::Entity& entity);

			static void submitVerticesIndices(
				ecs::RenderableComponent& ren, 
				std::vector<float>& vertices, 
				std::vector<uint32_t>& indices, 
				uint32_t& indicesmax, 
				int32_t& counter, 
				uint32_t& stride
			);

			void passLightToShader(ShaderOpenGL& shader);
			void passCameraToShader(ShaderOpenGL& shader);
		
			void updateTransforms(ecs::Scene* scene);
			void updateColors(ecs::Scene* scene);
			void updateLight(ecs::Scene* scene);
			void updateCamera(ecs::Scene* scene);
		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_H
