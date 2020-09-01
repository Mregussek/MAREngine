/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

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
