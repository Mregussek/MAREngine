/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FRAME_BUFFER_OPENGL_H
#define MAR_ENGINE_FRAME_BUFFER_OPENGL_H

#include "../../../../mar.h"
#include "FrameBuffer.h"


namespace mar {
	namespace graphics {


		class FrameBufferOpenGL : public FrameBuffer, std::enable_shared_from_this<FrameBufferOpenGL> {
			uint32_t m_id;
			uint32_t m_colorAttachment;
			uint32_t m_depthAttanchment;
			FrameBufferSpecification m_specification;
		
		public:
			FrameBufferOpenGL() = default;

			void initialize(const FrameBufferSpecification& spec) override;

			void bind() const override;

			void unbind() const override;

			void clear() const override;

			void close() override;

			// --- GET METHODS
			const uint32_t& getColorAttach() const override { return m_colorAttachment; }
			const uint32_t& getDepthAttach() const override { return m_depthAttanchment; }
			const FrameBufferSpecification& getSpecification() const override { return m_specification; }
		};


	}
}


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
