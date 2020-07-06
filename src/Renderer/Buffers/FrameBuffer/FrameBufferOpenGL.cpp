/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "../../../mar.h"
#include "FrameBufferOpenGL.h"
#include "../../../Debug/Log.h"

namespace mar {
	namespace graphics {


		void FrameBufferOpenGL::initialize(const FrameBufferSpecification& spec) {
			m_specification = spec;

			MAR_CORE_GL_FUNC( glGenTextures(1, &m_colorAttachment) );
			MAR_CORE_GL_FUNC( glBindTexture(GL_TEXTURE_2D, m_colorAttachment) );
			MAR_CORE_GL_FUNC( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (size_t)m_specification.width, (size_t)m_specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0) );
			MAR_CORE_GL_FUNC( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
			MAR_CORE_GL_FUNC( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
							  
			MAR_CORE_GL_FUNC( glGenRenderbuffers(1, &m_depthAttanchment) );
			MAR_CORE_GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttanchment) );
			MAR_CORE_GL_FUNC( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (size_t)m_specification.width, (size_t)m_specification.height) );
			MAR_CORE_GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, 0) );
							  
			MAR_CORE_GL_FUNC( glGenFramebuffers(1, &m_id) );
			MAR_CORE_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, m_id) );
			MAR_CORE_GL_FUNC( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0) );
			MAR_CORE_GL_FUNC( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttanchment) );
			
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				MAR_CORE_ERROR("Cannot load framebuffer!");
			}
			else {
				MAR_CORE_INFO("Framebuffer loaded successfully!");
			}

			MAR_CORE_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		}

		void FrameBufferOpenGL::bind() const {
			MAR_CORE_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, m_id) );
		}

		void FrameBufferOpenGL::unbind() const {
			MAR_CORE_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		}

		void FrameBufferOpenGL::clear() const {
			MAR_CORE_GL_FUNC( glViewport(0, 0, (const size_t)m_specification.width, (const size_t)m_specification.height) );
			MAR_CORE_GL_FUNC( glClearColor(0.22f, 0.69f, 0.87f, 1.0f) );
			MAR_CORE_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
		}

		void FrameBufferOpenGL::close() {
			MAR_CORE_GL_FUNC( glDeleteFramebuffers(1, &m_id) );
			MAR_CORE_GL_FUNC( glDeleteTextures(1, &m_colorAttachment) );
			MAR_CORE_GL_FUNC( glDeleteTextures(1, &m_depthAttanchment) );
		}


} }