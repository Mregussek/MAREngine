
#ifndef RENDERER_FACTORY_H
#define RENDERER_FACTORY_H

#include "Mesh/ElementBuffer/ElementBuffer.h"
#include "Mesh/VertexArray/VertexArray.h"
#include "Mesh/VertexBuffer/VertexBuffer.h"
#include "Mesh/VertexBuffer/VertexBufferLayout.h"
#include "Mesh/Texture/Texture.h"
#include "Mesh/Shapes/Shapes.h"
#include "Mesh/Shapes/Cube.h"

namespace mar {

	class RendererFactory {
	public:
		// This values should never be returned, cause we have derived factories
		virtual std::shared_ptr<ElementBuffer> createElementBuffer() { return nullptr; }
		virtual std::shared_ptr<VertexBuffer> createVertexBuffer() { return nullptr; }
		virtual std::shared_ptr<VertexBufferLayout> createVertexBufferLayout() { return nullptr; }
		virtual std::shared_ptr<VertexArray> createVertexArray() { return nullptr; }
		virtual std::shared_ptr<Texture> createTexture() { return nullptr; }
	};

}

#endif // !RENDERER_FACTORY_H


