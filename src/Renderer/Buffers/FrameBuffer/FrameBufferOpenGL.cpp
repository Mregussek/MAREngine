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

			glGenTextures(1, &m_colorAttachment);
			glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (size_t)m_specification.width, (size_t)m_specification.height, 0, GL_RGB,
						GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glGenRenderbuffers(1, &m_depthAttanchment);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttanchment);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (size_t)m_specification.width, (size_t)m_specification.height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			
			glGenFramebuffers(1, &m_id);
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttanchment);
			
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				MAR_CORE_ERROR("Cannot load framebuffer!");
			}
			else {
				MAR_CORE_INFO("Framebuffer loaded successfully!");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FrameBufferOpenGL::bind() const {
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		}

		void FrameBufferOpenGL::unbind() const {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FrameBufferOpenGL::clear() const {
			glViewport(0, 0, (const size_t)m_specification.width, (const size_t)m_specification.height);
			glClearColor(0.22f, 0.69f, 0.87f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void FrameBufferOpenGL::close() {
			glDeleteFramebuffers(1, &m_id);
			glDeleteTextures(1, &m_colorAttachment);
			glDeleteTextures(1, &m_depthAttanchment);
		}


} }