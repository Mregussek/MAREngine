
#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"
#include "../Shader/Shader.h"
#include "../ElementBuffer/ElementBuffer.h"
#include "../VertexArray/VertexArray.h"

class Renderer {
	size_t _numberOfPositions;
	size_t _numberOfIndices;

public:
	Renderer(const size_t& numberPositions, const size_t& numberIndices);

	void draw() const;

	void clear() const;
};

#endif // !RENDERER_H