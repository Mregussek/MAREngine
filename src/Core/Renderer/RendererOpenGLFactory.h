/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_OPENGL_FACTORY_H
#define RENDERER_OPENGL_FACTORY_H

#include "../../mar.h"
#include "../../Debug/Log.h"
#include "RendererFactory.h"


namespace mar {
	namespace graphics {


		class RendererOpenGLFactory : public RendererFactory {
		public:
			static Ref<RendererFactory> createFactory() {
				MAR_CORE_TRACE("FACTORY: creating factory!");
				return PointerFactory<RendererOpenGLFactory>::makeRef();
			}

			virtual Ref<ElementBuffer> createElementBuffer() override {
				MAR_CORE_TRACE("FACTORY: creating ElementBufferOpenGL!");
				return PointerFactory<ElementBufferOpenGL>::makeRef();
			}

			virtual Ref<VertexBuffer> createVertexBuffer() override {
				MAR_CORE_TRACE("FACTORY: creating VertexBufferOpenGL!");
				return PointerFactory<VertexBufferOpenGL>::makeRef();
			}

			virtual Ref<VertexBufferLayout> createVertexBufferLayout() override {
				MAR_CORE_TRACE("FACTORY: creating VertexBufferLayoutOpenGL!");
				return PointerFactory<VertexBufferLayoutOpenGL>::makeRef();
			}

			virtual Ref<VertexArray> createVertexArray() override {
				MAR_CORE_TRACE("FACTORY: creating VertexArrayOpenGL!");
				return PointerFactory<VertexArrayOpenGL>::makeRef();
			}

			virtual Ref<Texture> createTexture() override {
				MAR_CORE_TRACE("FACTORY: creating TextureOpenGL!");
				return PointerFactory<TextureOpenGL>::makeRef();
			}

			virtual Ref<Shader> createShader(ShaderType type) override {
				MAR_CORE_TRACE("FACTORY: creating ShaderOpenGL!");
				return PointerFactory<ShaderOpenGL>::makeRef();
			}

			virtual Ref<FrameBuffer> createFrameBuffer() override {
				MAR_CORE_TRACE("FACTORY: creating FrameBufferOpenGL!");
				return PointerFactory<FrameBufferOpenGL>::makeRef();
			}
		};


} }

#endif // !RENDERER_OPENGL_FACTORY_H
