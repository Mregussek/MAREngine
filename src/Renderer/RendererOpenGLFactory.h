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
			static std::vector<ShaderType> s_shaderTypes;
			static std::vector<Ref<ShaderOpenGL>> s_storedShaders;

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
				auto search = std::find(s_shaderTypes.begin(), s_shaderTypes.end(), type);

				if (search != s_shaderTypes.end()) {
					size_t dist = std::distance(s_shaderTypes.begin(), search);
					MAR_CORE_TRACE("Returning once created shader!");
					return s_storedShaders[dist];
				}

				s_shaderTypes.push_back(type);
				s_storedShaders.push_back(std::make_shared<ShaderOpenGL>());
				return s_storedShaders[s_storedShaders.size() - 1];
			}
			virtual Ref<FrameBuffer> createFrameBuffer() override {
				return std::make_shared<FrameBufferOpenGL>();
			}
		};


} }

#endif // !RENDERER_OPENGL_FACTORY_H
