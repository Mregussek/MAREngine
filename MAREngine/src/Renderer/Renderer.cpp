
#include "Renderer.h"

Renderer::Renderer(const size_t& numberPositions, const size_t& numberIndices)
	: _numberOfPositions(numberPositions),
	_numberOfIndices(numberIndices)
{
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::draw() const {
	if(_numberOfIndices == -1) glDrawArrays(GL_TRIANGLES, 0, 36);
	else glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() const {
	//glClearColor(0.65f, 0.65f, 0.65f, 1.0f); // light gray
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}