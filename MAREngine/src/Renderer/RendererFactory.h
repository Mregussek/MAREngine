/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_FACTORY_H
#define RENDERER_FACTORY_H

#include "Mesh/ElementBuffer/ElementBuffer.h"
#include "Mesh/ElementBuffer/ElementBufferOpenGL.h"

#include "Mesh/VertexArray/VertexArray.h"
#include "Mesh/VertexArray/VertexArrayOpenGL.h"

#include "Mesh/VertexBuffer/VertexBuffer.h"
#include "Mesh/VertexBuffer/VertexBufferOpenGL.h"

#include "Mesh/VertexBuffer/VertexBufferLayout.h"
#include "Mesh/VertexBuffer/VertexBufferLayoutOpenGL.h"

#include "Mesh/Texture/Texture.h"
#include "Mesh/Texture/TextureOpenGL.h"

#include "Shader/Shader.h"
#include "Shader/ShaderOpenGL.h"

#include "Mesh/Mesh.h"
#include "Mesh/Shapes/Shapes.h"
#include "Mesh/Shapes/Cube.h"
#include "Mesh/Shapes/Pyramid.h"

namespace mar {

	class RendererFactory {
	public:
		// This values should never be returned, cause we have derived factories
		virtual std::shared_ptr<ElementBuffer> createElementBuffer() 
		{ return nullptr; }
		virtual std::shared_ptr<VertexBuffer> createVertexBuffer() 
		{ return nullptr; }
		virtual std::shared_ptr<VertexBufferLayout> createVertexBufferLayout() 
		{ return nullptr; }
		virtual std::shared_ptr<VertexArray> createVertexArray() 
		{ return nullptr; }
		virtual std::shared_ptr<Texture> createTexture() 
		{ return nullptr; }
		virtual std::shared_ptr<Shader> createShader()
		{ return nullptr; }
	};

}

#endif // !RENDERER_FACTORY_H


