
#ifndef RENDERER_H
#define RENDERER_H

#include "mre.h"
#include "Shader.h"
#include "ElementBuffer.h"
#include "VertexArray.h"

class Renderer {
	size_t _numberOfIndices;
public:
	Renderer(const size_t& number) : _numberOfIndices(number) {}

	void draw(const VertexArray& va, const ElementBuffer& eb, const Shader& shader) const;

	void clear() const;
};

#endif // !RENDERER_H