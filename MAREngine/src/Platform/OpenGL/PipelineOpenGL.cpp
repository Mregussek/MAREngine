/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "PipelineOpenGL.h"
#include "../../Core/graphics/Mesh/Vertex.h"
#include "../../Debug/Logger.h"


namespace marengine {


	// ---- PUBLIC METHODS ---- //

	void PipelineOpenGL::initialize(uint32_t memoryVBO, uint32_t memoryEBO) {
		m_vboAllocMemory = memoryVBO;
		m_eboAllocMemory = memoryEBO;

		createEBO();
		createVAO();
		createVBO();
		processLayout();
	}

	void PipelineOpenGL::close() {
		destroyVAO();
		destroyVBO();
		destroyEBO();
	}

	void PipelineOpenGL::update(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) const {
		const uint32_t vertSize{ vertices.size() * sizeof(Vertex) };
		const uint32_t indiSize{ indices.size() * sizeof(uint32_t) };

		PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, vertices.data()) );
		PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indiSize, indices.data()) );
	}

	void PipelineOpenGL::reset() const {
		PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, 0, nullptr) );
		PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, nullptr) );
	}

	void PipelineOpenGL::bind() const {
		PLATFORM_GL_FUNC( glBindVertexArray(m_vao) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );
	}

	void PipelineOpenGL::unbind() const {
		PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, 0) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
		PLATFORM_GL_FUNC( glBindVertexArray(0) );
	}

	// ---- PRIVATE METHODS ---- //

	void PipelineOpenGL::createVAO() {
		PLATFORM_GL_FUNC(glGenVertexArrays(1, &m_vao));
		PLATFORM_GL_FUNC(glBindVertexArray(m_vao));
	}

	void PipelineOpenGL::destroyVAO() {
		PLATFORM_GL_FUNC(glDeleteVertexArrays(1, &m_vao));
	}

	void PipelineOpenGL::createVBO() {
		PLATFORM_GL_FUNC( glGenBuffers(1, &m_vbo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
		PLATFORM_GL_FUNC( glBufferData(GL_ARRAY_BUFFER, m_vboAllocMemory, nullptr, GL_DYNAMIC_DRAW) );
	}

	void PipelineOpenGL::destroyVBO() {
		PLATFORM_GL_FUNC(glDeleteBuffers(1, &m_vbo));
	}

	void PipelineOpenGL::createEBO() {
		PLATFORM_GL_FUNC( glGenBuffers(1, &m_ebo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );
		PLATFORM_GL_FUNC( glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_eboAllocMemory, nullptr, GL_DYNAMIC_DRAW) );
	}

	void PipelineOpenGL::destroyEBO() {
		PLATFORM_GL_FUNC(glDeleteBuffers(1, &m_ebo));
	}

	void PipelineOpenGL::processLayout() const {
		struct VertexBufferElement {
			uint32_t type;
			uint32_t count;
			unsigned char normalized;
		};

		constexpr std::array<const VertexBufferElement, 4> elements{
			VertexBufferElement{ GL_FLOAT, 3, GL_FALSE },
			VertexBufferElement{ GL_FLOAT, 3, GL_FALSE },
			VertexBufferElement{ GL_FLOAT, 2, GL_FALSE },
			VertexBufferElement{ GL_FLOAT, 1, GL_FALSE }
		};
		constexpr uint32_t stride{
			elements[0].count * sizeof(elements[0].type) +
			elements[1].count * sizeof(elements[1].type) +
			elements[2].count * sizeof(elements[2].type) +
			elements[3].count * sizeof(elements[3].type)
		};

		uint32_t offset = 0;
		for (uint32_t i = 0; i < elements.size(); i++) {
			PLATFORM_GL_FUNC( glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (const void*)offset) );
			PLATFORM_GL_FUNC( glEnableVertexAttribArray(i) );

			offset += elements[i].count * sizeof(elements[i].type);
		}
	}


}
