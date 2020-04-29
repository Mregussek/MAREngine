
#include "Renderer.h"

void Renderer::draw(const VertexArray& va, const ElementBuffer& eb, const Shader& shader) const {
	shader.bind();

	va.bind();
	eb.bind();

	glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() const {
	//glClearColor(0.65f, 0.65f, 0.65f, 1.0f); // light gray
	glClear(GL_COLOR_BUFFER_BIT);
}