/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexArrayOpenGL.h"

namespace mar {
	namespace graphics {


		void VertexArrayOpenGL::initializeArrayBuffer() {
			MAR_CORE_GL_FUNC(glGenVertexArrays(1, &_rendererId));
			MAR_CORE_GL_FUNC(glBindVertexArray(_rendererId));
		}

		void VertexArrayOpenGL::closeArrayBuffer() {
			MAR_CORE_GL_FUNC(glDeleteVertexArrays(1, &_rendererId));
		}

		void VertexArrayOpenGL::addBuffer(const VertexBufferLayout<VertexBufferLayoutOpenGL>& layout) {
			const auto& elements = layout.getElements();
			uint32_t offset = 0;

			for (uint32_t i = 0; i < elements.size(); i++) {
				MAR_CORE_GL_FUNC( glVertexAttribPointer(i, elements[i]._count, elements[i]._type, elements[i]._normalized, layout.getStride(), (const void*)offset) );

				MAR_CORE_GL_FUNC(glEnableVertexAttribArray(i));
				offset += elements[i]._count * sizeof(elements[i]._type);
			}
		}

		void VertexArrayOpenGL::bind() const {
			MAR_CORE_GL_FUNC(glBindVertexArray(_rendererId));
		}

		void VertexArrayOpenGL::unbind() const {
			MAR_CORE_GL_FUNC(glBindVertexArray(0));
		}


} }
