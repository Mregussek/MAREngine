
#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"
#include "../Shader/Shader.h"
#include "../ElementBuffer/ElementBuffer.h"
#include "../VertexArray/VertexArray.h"

class Renderer {
	size_t _numberOfIndices;
public:
	Renderer(const size_t& number) : _numberOfIndices(number) {}

	void draw(const VertexArray& va, const ElementBuffer& eb, const Shader& shader) const;

	void clear() const;
};

#endif // !RENDERER_H