/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
#define MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H


#include "../../../mar.h"
#include "Renderer.h"
#include "RenderPipeline.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"


namespace mar {
	namespace graphics {


		class RendererBatch {
		public:

			void initialize();
			void close();
			void draw(RenderPipeline& render_pip);

		private:

			void passTexturesToShader(RenderPipeline& ren);
			void passLightToShader(RenderPipeline& ren);
			void passCameraToShader(RenderPipeline& ren);

			platforms::PipelineOpenGL m_buffers;
			platforms::ShaderOpenGL m_shader;
			platforms::TextureOpenGL m_texture;
		};


} }

#endif // !MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
