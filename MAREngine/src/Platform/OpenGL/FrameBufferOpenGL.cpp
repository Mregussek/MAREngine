/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "FrameBufferOpenGL.h"


namespace mar {
	namespace platforms {


		void FrameBufferOpenGL::initialize(const FrameBufferSpecification& spec) {
			m_specification = spec;

			PLATFORM_GL_FUNC( glGenTextures(1, &m_colorAttachment) );
			PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_2D, m_colorAttachment) );
			PLATFORM_GL_FUNC( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (size_t)m_specification.width, (size_t)m_specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0) );
			PLATFORM_GL_FUNC( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
			PLATFORM_GL_FUNC( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
							  
			PLATFORM_GL_FUNC( glGenRenderbuffers(1, &m_depthAttanchment) );
			PLATFORM_GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttanchment) );
			PLATFORM_GL_FUNC( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (size_t)m_specification.width, (size_t)m_specification.height) );
			PLATFORM_GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, 0) );
							  
			PLATFORM_GL_FUNC( glGenFramebuffers(1, &m_id) );
			PLATFORM_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, m_id) );
			PLATFORM_GL_FUNC( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0) );
			PLATFORM_GL_FUNC( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttanchment) );
			
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				PLATFORM_ERROR("FRAMEBUFFER_OPENGL: Cannot load framebuffer!");
			}
			else {
				PLATFORM_INFO("FRAMEBUFFER_OPENGL: loaded successfully - {}!", m_id);
			}

			PLATFORM_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		}

		void FrameBufferOpenGL::bind() const {
			PLATFORM_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, m_id) );

			PLATFORM_TRACE("FRAMEBUFFER_OPENGL: Binding - {}!", m_id);
		}

		void FrameBufferOpenGL::unbind() const {
			PLATFORM_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

			PLATFORM_TRACE("FRAMEBUFFER_OPENGL: Unbinding FBO");
		}

		void FrameBufferOpenGL::clear() const {
			PLATFORM_GL_FUNC( glViewport(0, 0, (const size_t)m_specification.width, (const size_t)m_specification.height) );
			PLATFORM_GL_FUNC( glClearColor(m_specification.backgroundColor.x, m_specification.backgroundColor.y, m_specification.backgroundColor.z, 1.0f) );
			PLATFORM_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );

			PLATFORM_TRACE("FRAMEBUFFER_OPENGL: clearing!");
		}

		void FrameBufferOpenGL::close() {
			PLATFORM_TRACE("FRAMEBUFFER_OPENGL: deleting {}!", m_id);

			PLATFORM_GL_FUNC( glDeleteFramebuffers(1, &m_id) );
			PLATFORM_GL_FUNC( glDeleteTextures(1, &m_colorAttachment) );
			PLATFORM_GL_FUNC( glDeleteTextures(1, &m_depthAttanchment) );
		}


} }