/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FRAME_BUFFER_H
#define MAR_ENGINE_FRAME_BUFFER_H

#include "../../../../../mar.h"
#include "../../../GraphicsLogs.h"


namespace mar {
	namespace graphics {


		struct FrameBufferSpecification {
			float width{ 0.f };
			float height{ 0.f };
			uint32_t samples = 1;

			bool swapChainTarget = false;
			
			FrameBufferSpecification() = default;
			FrameBufferSpecification(float w, float h) : width(w), height(h) { }
		};

		template<typename FRMBUF>
		class FrameBuffer {
			FRMBUF m_framebuffer;

		public:
			FrameBuffer() = default;

			void initialize(const FrameBufferSpecification& spec) { 
				m_framebuffer.initialize(spec);
			}

			void bind() const { 
				m_framebuffer.bind();
			}

			void unbind() const { 
				m_framebuffer.unbind();
			}

			void clear() const  { 
				m_framebuffer.clear();
			}

			void close() { 
				m_framebuffer.close();
			}

			const uint32_t& getColorAttach() const { 
				return m_framebuffer.getColorAttach();
			}

			const uint32_t& getDepthAttach() const { 
				return m_framebuffer.getDepthAttach(); 
			}

			const FrameBufferSpecification& getSpecification() const { 
				return m_framebuffer.getSpecification();
			}
		};


} }


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
