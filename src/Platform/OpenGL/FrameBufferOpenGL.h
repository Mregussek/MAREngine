/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FRAME_BUFFER_OPENGL_H
#define MAR_ENGINE_FRAME_BUFFER_OPENGL_H

#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace graphics {


		struct FrameBufferSpecification {
			maths::vec3 backgroundColor{ 0.22f, 0.69f, 0.87f };
			float width{ 0.f };
			float height{ 0.f };
			uint32_t samples = 1;

			bool swapChainTarget = false;

			FrameBufferSpecification() = default;
			FrameBufferSpecification(float w, float h) : width(w), height(h) { }
		};


		class FrameBufferOpenGL {
			uint32_t m_id;
			uint32_t m_colorAttachment;
			uint32_t m_depthAttanchment;
			FrameBufferSpecification m_specification;
		
		public:
			FrameBufferOpenGL() = default;

			void initialize(const FrameBufferSpecification& spec);

			void bind() const;

			void unbind() const;

			void clear() const;

			void close();

			// --- GET METHODS
			const uint32_t& getColorAttach() const { return m_colorAttachment; }
			const uint32_t& getDepthAttach() const { return m_depthAttanchment; }
			FrameBufferSpecification& getSpecification() { return m_specification; }
		};


	}
}


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
