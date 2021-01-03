/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "PipelineOpenGL.h"


namespace marengine {


	// ---- PUBLIC METHODS ---- //

	void PipelineOpenGL::initialize(uint32_t memoryVBO, uint32_t memoryEBO) {
		m_vboAllocMemory = memoryVBO;
		m_eboAllocMemory = memoryEBO;

		createEBO();
		createVAO();
		createVBO();
		processLayout();

		PLATFORM_INFO("PIPELINE_OPENGL: initialized Pipeline, VBO = {}, EBO = {}, VAO = {}", m_vbo, m_ebo, m_vao);
	}

	void PipelineOpenGL::close() {
		PLATFORM_TRACE("PIPELINE_OPENGL: closing VBO = {}, EBO = {}, VAO = {}", m_vbo, m_ebo, m_vao);

		destroyVAO();
		destroyVBO();
		destroyEBO();

		PLATFORM_INFO("PIPELINE_OPENGL: pipeline is closed!");
	}

	void PipelineOpenGL::update(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) const {
		const uint32_t vertSize{ vertices.size() * sizeof(Vertex) };
		const uint32_t indiSize{ indices.size() * sizeof(uint32_t) };

		PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, vertices.data()) );
		PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indiSize, indices.data()) );
	
		PLATFORM_TRACE("PIPELINE_OPENGL: updated vertices at VBO with size = {}, memory (bytes) = {} and indices at EBO size = {} memory (bytes) = {}", 
			vertices.size(), vertSize, indices.size(), indiSize);
	}

	void PipelineOpenGL::reset() const {
		PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, 0, nullptr) );
		PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, nullptr) );
		
		PLATFORM_TRACE("PIPELINE_OPENGL: reseting data at VBO - {}, EBO - {}", m_vbo, m_ebo);
	}

	void PipelineOpenGL::bind() const {
		PLATFORM_GL_FUNC( glBindVertexArray(m_vao) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );

		PLATFORM_TRACE("PIPELINE_OPENGL: bind(), VAO - {}, VBO - {}, EBO - {}", m_vao, m_vbo, m_ebo);
	}

	void PipelineOpenGL::unbind() const {
		PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, 0) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
		PLATFORM_GL_FUNC( glBindVertexArray(0) );

		PLATFORM_TRACE("PIPELINE_OPENGL: ubind(), VAO - {}, VBO - {}, EBO - {}", m_vao, m_vbo, m_ebo);
	}

	// ---- PRIVATE METHODS ---- //

	void PipelineOpenGL::createVAO() {
		PLATFORM_TRACE("PIPELINE_OPENGL: creating VAO...");

		PLATFORM_GL_FUNC(glGenVertexArrays(1, &m_vao));
		PLATFORM_GL_FUNC(glBindVertexArray(m_vao));

		PLATFORM_INFO("PIPELINE_OPENGL: initialized VAO {}!", m_vao);
	}

	void PipelineOpenGL::destroyVAO() {
		PLATFORM_TRACE("PIPELINE_OPENGL: closing VAO {}...", m_vao);
		PLATFORM_GL_FUNC(glDeleteVertexArrays(1, &m_vao));
	}

	void PipelineOpenGL::createVBO() {
		PLATFORM_TRACE("PIPELINE_OPENGL: creating VBO...");

		PLATFORM_GL_FUNC( glGenBuffers(1, &m_vbo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
		PLATFORM_GL_FUNC( glBufferData(GL_ARRAY_BUFFER, m_vboAllocMemory, nullptr, GL_DYNAMIC_DRAW) );

		PLATFORM_INFO("PIPELINE_OPENGL: initialized VBO {} and allocated {} bytes!", m_vbo, m_vboAllocMemory);
	}

	void PipelineOpenGL::destroyVBO() {
		PLATFORM_TRACE("PIPELINE_OPENGL: closing VBO {}...", m_vbo);
		PLATFORM_GL_FUNC(glDeleteBuffers(1, &m_vbo));
	}

	void PipelineOpenGL::createEBO() {
		PLATFORM_TRACE("PIPELINE_OPENGL: creating EBO...");

		PLATFORM_GL_FUNC( glGenBuffers(1, &m_ebo) );
		PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );
		PLATFORM_GL_FUNC( glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_eboAllocMemory, nullptr, GL_DYNAMIC_DRAW) );

		PLATFORM_INFO("PIPELINE_OPENGL: initialized EBO {} and allocated {}!", m_ebo, m_eboAllocMemory);
	}

	void PipelineOpenGL::destroyEBO() {
		PLATFORM_TRACE("PIPELINE_OPENGL: closing EBO {}...", m_ebo);
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
			3 * sizeof(GL_FLOAT) + 3 * sizeof(GL_FLOAT) +
			2 * sizeof(GL_FLOAT) + 1 * sizeof(GL_FLOAT)
		};

		uint32_t offset = 0;

		for (uint32_t i = 0; i < elements.size(); i++) {
			PLATFORM_GL_FUNC( glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (const void*)offset) );
			PLATFORM_GL_FUNC( glEnableVertexAttribArray(i) );

			offset += elements[i].count * sizeof(elements[i].type);

			PLATFORM_TRACE("PIPELINE_OPENGL: adding attribute to VAO - {}! glVertexAttribPointer({}, {}, {}, {}, {}, {})"
				, m_vao, i, elements[i].count, elements[i].type, elements[i].normalized, stride, offset);
		}

		PLATFORM_INFO("PIPELINE_OPENGL: added layout {3, 3, 2, 1} to VAO {}", m_vao);
	}


}
