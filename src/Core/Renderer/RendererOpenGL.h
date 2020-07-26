/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_OPENGL_H
#define MAR_ENGINE_RENDERER_OPENGL_H

#include "../../mar.h"
#include "Renderer.h"


namespace mar {
	namespace graphics {

		struct RendererStatistics;


		class RendererOpenGL {
			// --- Buffers
			VertexBufferOpenGL m_vbo;
			VertexBufferLayoutOpenGL m_layout;
			VertexArrayOpenGL m_vao;
			ElementBufferOpenGL m_ebo;
			ShaderOpenGL m_mainShader;
			// --- Run-time
			static Light* s_light;
			// --- Knowledge about state of Renderer
			bool m_initialized = false;	// check, if renderer is initialized
			static RendererStatistics s_stats;

		public:
			RendererOpenGL() = default;

			void close();

			void initialize(const std::vector<uint32_t>& layout, const ShaderType type);

			void draw(Mesh* mesh);

		private:
			void updateMeshData(Mesh* mesh);
			void updateGUIData();
			void updateCameraData();
			void updateLightData(Light* light);

		public:
			// --- SET METHODS --- //
			static void setLight(Light* light) { s_light = light; }

			// --- GET METHODS --- // 
			inline static RendererStatistics& getStatistics() { return s_stats; }
		};


} }

#endif // !MAR_ENGINE_RENDERER_OPENGL_H