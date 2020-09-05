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


namespace mar {
	namespace platforms {


		PipelineOpenGL::PipelineOpenGL()
			: m_vao(0),
			m_vbo(0),
			m_ebo(0),
			m_vboAllocMemory(0),
			m_eboAllocMemory(0),
			m_initialized(0)
		{

		}

		void PipelineOpenGL::initialize(uint32_t vbo_memory, uint32_t ebo_memory) {
			if (m_initialized) {
				PLATFORM_WARN("PIPELINE_OPENGL: is already initialized with VAO {} EBO {} VBO {}!", m_vao, m_ebo, m_vbo);

				return;
			}
			
			m_eboAllocMemory = ebo_memory;
			PLATFORM_GL_FUNC( glGenBuffers(1, &m_ebo) );
			PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );
			PLATFORM_GL_FUNC( glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_eboAllocMemory, nullptr, GL_DYNAMIC_DRAW) );
			
			PLATFORM_TRACE("PIPELINE_OPENGL: initialized EBO {} with memory {}!", m_ebo, m_eboAllocMemory);

			PLATFORM_GL_FUNC( glGenVertexArrays(1, &m_vao) );
			PLATFORM_GL_FUNC( glBindVertexArray(m_vao) );

			PLATFORM_TRACE("PIPELINE_OPENGL: initialized VAO {}!", m_vao);

			m_vboAllocMemory = vbo_memory;
			PLATFORM_GL_FUNC(glGenBuffers(1, &m_vbo));
			PLATFORM_GL_FUNC(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
			PLATFORM_GL_FUNC(glBufferData(GL_ARRAY_BUFFER, m_vboAllocMemory, nullptr, GL_DYNAMIC_DRAW));
			
			PLATFORM_TRACE("PIPELINE_OPENGL: initialized VBO {} with memory {}!", m_vbo, m_vboAllocMemory);
		
			m_initialized = true;

			PLATFORM_INFO("PIPELINE_OPENGL: initialized Pipeline!");
		}

		void PipelineOpenGL::close() {
			if (!m_initialized) {
				PLATFORM_WARN("PIPELINE_OPENGL: is not even initialized!");
				return;
			}

			PLATFORM_TRACE("PIPELINE_OPENGL: Closing VAO {}!", m_vao);
			PLATFORM_GL_FUNC( glDeleteVertexArrays(1, &m_vao) );

			PLATFORM_TRACE("PIPELINE_OPENGL: closing VBO {}!", m_vbo);
			PLATFORM_GL_FUNC( glDeleteBuffers(1, &m_vbo) );

			PLATFORM_TRACE("PIPELINE_OPENGL: closing EBO {}!", m_ebo);
			PLATFORM_GL_FUNC( glDeleteBuffers(1, &m_ebo) );

			m_initialized = false;

			PLATFORM_INFO("PIPELINE_OPENGL: pipeline is closed!");
		}

		void PipelineOpenGL::processLayout() {
			struct VertexBufferElement {
				uint32_t type;
				uint32_t count;
				unsigned char normalized;
			};

			std::vector<VertexBufferElement> elements;
			uint32_t stride = 0;

			const std::vector<uint32_t> layout{ 3, 3, 2, 1 };

			for (auto l : layout) {
				elements.push_back({ GL_FLOAT, l, GL_FALSE });
				stride += l * sizeof(GL_FLOAT);
			}

			uint32_t offset = 0;

			for (uint32_t i = 0; i < elements.size(); i++) {
				PLATFORM_GL_FUNC(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (const void*)offset));
				PLATFORM_GL_FUNC(glEnableVertexAttribArray(i));

				offset += elements[i].count * sizeof(elements[i].type);

				PLATFORM_TRACE("PIPELINE_OPENGL: adding attribute to VAO - {}! glVertexAttribPointer({}, {}, {}, {}, {}, {})"
					, m_vao, i, elements[i].count, elements[i].type, elements[i].normalized, stride, offset);
			}

			PLATFORM_INFO("PIPELINE_OPENGL: added layout {3, 3, 2, 1} to VAO {}", m_vao);
		}

		void PipelineOpenGL::updateBuffers(const std::vector<float>& vertices, const std::vector<uint32_t>& indices) {
			uint32_t vert_size = vertices.size() * sizeof(float);
			uint32_t indi_size = indices.size() * sizeof(uint32_t);

			PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, vert_size, vertices.data()) );
			PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indi_size, indices.data()) );
		
			PLATFORM_TRACE("PIPELINE_OPENGL: updated vertices size = {} memory = {} and indices size = {} memory = {}", vertices.size(), vert_size, indices.size(), indi_size );
		}

		void PipelineOpenGL::resetBuffers() {
			PLATFORM_GL_FUNC( glBufferSubData(GL_ARRAY_BUFFER, 0, 0, nullptr) );
			PLATFORM_GL_FUNC( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, nullptr) );
			
			PLATFORM_TRACE("PIPELINE_OPENGL: reseting VBO {} and EBO {}", m_vbo, m_ebo);
		}

		void PipelineOpenGL::bind() {
			PLATFORM_GL_FUNC( glBindVertexArray(m_vao) );
			PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
			PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );

			PLATFORM_TRACE("PIPELINE_OPENGL: bounded VAO {} VBO {} EBO {}", m_vao, m_vbo, m_ebo);
		}

		void PipelineOpenGL::unbind() {
			PLATFORM_GL_FUNC( glBindBuffer(GL_ARRAY_BUFFER, 0) );
			PLATFORM_GL_FUNC( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
			PLATFORM_GL_FUNC( glBindVertexArray(0) );

			PLATFORM_TRACE("PIPELINE_OPENGL: unbounded");
		}

} }