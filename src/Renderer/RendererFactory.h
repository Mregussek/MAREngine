/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_FACTORY_H
#define RENDERER_FACTORY_H

#include "../mar.h"

#include "Buffers/ElementBuffer/ElementBuffer.h"
#include "Buffers/ElementBuffer/ElementBufferOpenGL.h"
	
#include "Buffers/VertexArray/VertexArray.h"
#include "Buffers/VertexArray/VertexArrayOpenGL.h"

#include "Buffers/VertexBuffer/VertexBuffer.h"
#include "Buffers/VertexBuffer/VertexBufferOpenGL.h"
	
#include "Buffers/VertexBuffer/VertexBufferLayout.h"
#include "Buffers/VertexBuffer/VertexBufferLayoutOpenGL.h"

#include "Buffers/FrameBuffer/FrameBuffer.h"
#include "Buffers/FrameBuffer/FrameBufferOpenGL.h"

#include "Mesh/Texture/Texture.h"
#include "Mesh/Texture/TextureOpenGL.h"

#include "Shader/Shader.h"
#include "Shader/ShaderOpenGL.h"

#include "Scene/Scene.h"

#include "Mesh/Shapes/ShapeManipulator.h"
#include "Mesh/MeshCreator.h"


namespace mar {
	namespace graphics {


		class RendererFactory {
		public:
			// This values should never be returned, cause we have derived factories
			static Ref<RendererFactory> createFactory()
			{
				return nullptr;
			}
			virtual Ref<ElementBuffer> createElementBuffer()
			{
				return nullptr;
			}
			virtual Ref<VertexBuffer> createVertexBuffer()
			{
				return nullptr;
			}
			virtual Ref<VertexBufferLayout> createVertexBufferLayout()
			{
				return nullptr;
			}
			virtual Ref<VertexArray> createVertexArray()
			{
				return nullptr;
			}
			virtual Ref<Texture> createTexture()
			{
				return nullptr;
			}
			virtual Ref<Shader> createShader(ShaderType type)
			{
				return nullptr;
			}
			virtual Ref<FrameBuffer> createFrameBuffer() 
			{
				return nullptr;
			}
		};


} }

#endif // !RENDERER_FACTORY_H


