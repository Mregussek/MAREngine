/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_OPENGL_FACTORY_H
#define RENDERER_OPENGL_FACTORY_H

#include "../../mar.h"
#include "RendererFactory.h"

namespace mar {
	namespace graphics {


		class RendererOpenGLFactory : public RendererFactory {
		public:
			static Ref<RendererFactory> createFactory() {
				return PointerFactory<RendererOpenGLFactory>::makeRef();
			}

			virtual Ref<ElementBuffer> createElementBuffer() override {
				return PointerFactory<ElementBufferOpenGL>::makeRef();
			}

			virtual Ref<VertexBuffer> createVertexBuffer() override {
				return PointerFactory<VertexBufferOpenGL>::makeRef();
			}

			virtual Ref<VertexBufferLayout> createVertexBufferLayout() override {
				return PointerFactory<VertexBufferLayoutOpenGL>::makeRef();
			}

			virtual Ref<VertexArray> createVertexArray() override {
				return PointerFactory<VertexArrayOpenGL>::makeRef();
			}

			virtual Ref<Texture> createTexture() override {
				return PointerFactory<TextureOpenGL>::makeRef();
			}

			virtual Ref<Shader> createShader(ShaderType type) override {
				return PointerFactory<ShaderOpenGL>::makeRef();
			}

			virtual Ref<FrameBuffer> createFrameBuffer() override {
				return PointerFactory<FrameBufferOpenGL>::makeRef();
			}
		};


} }

#endif // !RENDERER_OPENGL_FACTORY_H
