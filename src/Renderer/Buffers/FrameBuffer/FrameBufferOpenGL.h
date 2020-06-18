/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FRAME_BUFFER_OPENGL_H
#define MAR_ENGINE_FRAME_BUFFER_OPENGL_H

#include "../../../mar.h"
#include "FrameBuffer.h"


namespace mar {
	namespace graphics {


		class FrameBufferOpenGL : public FrameBuffer, std::enable_shared_from_this<FrameBufferOpenGL> {
			unsigned int m_id;
			unsigned int m_colorAttachment;
			unsigned int m_depthAttanchment;
			FrameBufferSpecification m_specification;
		
		public:
			FrameBufferOpenGL() = default;

			void initialize(const FrameBufferSpecification& spec) override;

			void bind() const override;

			void unbind() const override;

			void clear() const override;

			void close() override;

			// --- GET METHODS
			const unsigned int& getColorAttach() const override { return m_colorAttachment; }
			const unsigned int& getDepthAttach() const override { return m_depthAttanchment; }
			const FrameBufferSpecification& getSpecification() const override { return m_specification; }
		};


	}
}


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
