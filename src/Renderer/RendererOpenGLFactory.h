/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_OPENGL_FACTORY_H
#define RENDERER_OPENGL_FACTORY_H

#include "../mar.h"
#include "RendererFactory.h"

namespace mar {
	namespace graphics {


		class RendererOpenGLFactory : public RendererFactory {
		public:
			static Ref<RendererFactory> createFactory() {
				return std::make_shared<RendererOpenGLFactory>();
			}

			virtual Ref<ElementBuffer> createElementBuffer() override {
				return std::make_shared<ElementBufferOpenGL>();
			}

			virtual Ref<VertexBuffer> createVertexBuffer() override {
				return std::make_shared<VertexBufferOpenGL>();
			}

			virtual Ref<VertexBufferLayout> createVertexBufferLayout() override {
				return std::make_shared<VertexBufferLayoutOpenGL>();
			}

			virtual Ref<VertexArray> createVertexArray() override {
				return std::make_shared<VertexArrayOpenGL>();
			}

			virtual Ref<Texture> createTexture() override {
				return std::make_shared<TextureOpenGL>();
			}

			virtual Ref<Shader> createShader(ShaderType type) override {
				return std::make_shared<ShaderOpenGL>();
			}

			virtual Ref<FrameBuffer> createFrameBuffer() override {
				return std::make_shared<FrameBufferOpenGL>();
			}
		};


} }

#endif // !RENDERER_OPENGL_FACTORY_H
