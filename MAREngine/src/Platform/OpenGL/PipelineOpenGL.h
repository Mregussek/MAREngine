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


#ifndef MAR_ENGINE_PLATFORM_OPENGL_PIPELINE_H
#define MAR_ENGINE_PLATFORM_OPENGL_PIPELINE_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		class PipelineOpenGL {
			uint32_t m_vao;
			uint32_t m_vbo;
			uint32_t m_ebo;
			
			uint32_t m_vboAllocMemory;
			uint32_t m_eboAllocMemory;

			bool m_initialized;

		public:

			PipelineOpenGL();

			void initialize(uint32_t vbo_memory, uint32_t ebo_memory);
			void close();

			void processLayout();

			void updateBuffers(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
			void resetBuffers();

			void bind();
			void unbind();
		};	


} }


#endif // !MAR_ENGINE_PLATFORM_OPENGL_PIPELINE_H
