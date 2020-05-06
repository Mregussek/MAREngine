
#ifndef RENDERER_OPENGL_FACTORY_H
#define RENDERER_OPENGL_FACTORY_H

#include "RendererFactory.h"

namespace mar {

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
	};
}

#endif // !RENDERER_OPENGL_FACTORY_H
