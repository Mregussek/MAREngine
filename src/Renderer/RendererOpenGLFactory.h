/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_OPENGL_FACTORY_H
#define RENDERER_OPENGL_FACTORY_H

#include "RendererFactory.h"

namespace mar {
	namespace graphics {


		class RendererOpenGLFactory : public RendererFactory {
		public:
			virtual std::shared_ptr<ElementBuffer> createElementBuffer() override {
				return std::make_shared<ElementBufferOpenGL>();
			}

			virtual std::shared_ptr<VertexBuffer> createVertexBuffer() override {
				return std::make_shared<VertexBufferOpenGL>();
			}

			virtual std::shared_ptr<VertexBufferLayout> createVertexBufferLayout() override {
				return std::make_shared<VertexBufferLayoutOpenGL>();
			}

			virtual std::shared_ptr<VertexArray> createVertexArray() override {
				return std::make_shared<VertexArrayOpenGL>();
			}

			virtual std::shared_ptr<Texture> createTexture() override {
				return std::make_shared<TextureOpenGL>();
			}

			virtual std::shared_ptr<Shader> createShader() override {
				return std::make_shared<ShaderOpenGL>();
			}
		};


} }

#endif // !RENDERER_OPENGL_FACTORY_H
