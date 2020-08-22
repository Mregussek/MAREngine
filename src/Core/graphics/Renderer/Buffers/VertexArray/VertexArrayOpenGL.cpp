/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "VertexArrayOpenGL.h"


namespace mar {
	namespace graphics {


		void VertexArrayOpenGL::initialize() {
			MAR_CORE_GL_FUNC(glGenVertexArrays(1, &_rendererId));
			MAR_CORE_GL_FUNC(glBindVertexArray(_rendererId));

			GRAPHICS_INFO("VERTEXARRAY_OPENGL: initialized {}!", _rendererId);
		}

		void VertexArrayOpenGL::close() {
			GRAPHICS_INFO("VERTEXARRAY_OPENGL: Closing {}!", _rendererId);

			MAR_CORE_GL_FUNC(glDeleteVertexArrays(1, &_rendererId));
		}

		void VertexArrayOpenGL::addBuffer(const VertexBufferLayoutOpenGL& layout) {
			const auto& elements = layout.getElements();
			uint32_t offset = 0;

			for (uint32_t i = 0; i < elements.size(); i++) {
				MAR_CORE_GL_FUNC( glVertexAttribPointer(i, elements[i]._count, elements[i]._type, elements[i]._normalized, layout.getStride(), (const void*)offset) );

				MAR_CORE_GL_FUNC( glEnableVertexAttribArray(i) );

				offset += elements[i]._count * sizeof(elements[i]._type);

				GRAPHICS_TRACE("VERTEXARRAY_OPENGL: adding buffer to VAO - {}! glVertexAttribPointer({}, {}, {}, {}, {}, {})"
					, _rendererId, i, elements[i]._count, elements[i]._type, elements[i]._normalized, layout.getStride(), offset);
			}

			GRAPHICS_INFO("VERTEXARRAY_OPENGL: added buffer to VAO - {}!", _rendererId);
		}

		void VertexArrayOpenGL::bind() const {
			MAR_CORE_GL_FUNC( glBindVertexArray(_rendererId) );

			GRAPHICS_TRACE("VERTEXARRAY_OPENGL: binding EBO {}!", _rendererId);
		}

		void VertexArrayOpenGL::unbind() const {
			MAR_CORE_GL_FUNC( glBindVertexArray(0) );

			GRAPHICS_TRACE("VERTEXARRAY_OPENGL: unbiding {}!", _rendererId);
		}


} }
