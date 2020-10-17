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


#include "FrameBufferOpenGL.h"


namespace mar::platforms {


	// ---- PUBLIC METHODS ---- //

	void FramebufferOpenGL::initialize(float width, float height) {
		m_specification = FramebufferSpecification(width, height);

		createColorAttachment();
		createDepthAttachment();
		createFramebuffer();		 
		checkCreationStatus();

		unbind();
	}

	void FramebufferOpenGL::close() {
		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: deleting {}!", m_id);

		PLATFORM_GL_FUNC(glDeleteFramebuffers(1, &m_id));
		PLATFORM_GL_FUNC(glDeleteTextures(1, &m_colorAttachment));
		PLATFORM_GL_FUNC(glDeleteTextures(1, &m_depthAttanchment));
	}

	void FramebufferOpenGL::bind() const {
		PLATFORM_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, m_id) );

		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: Binding - {}!", m_id);
	}

	void FramebufferOpenGL::unbind() const {
		PLATFORM_GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: Unbinding FBO");
	}

	void FramebufferOpenGL::clear() const {
		PLATFORM_GL_FUNC( glViewport(0, 0, (const size_t)m_specification.width, (const size_t)m_specification.height) );
		PLATFORM_GL_FUNC( glClearColor(m_specification.backgroundColor.x, m_specification.backgroundColor.y, m_specification.backgroundColor.z, 1.0f) );
		PLATFORM_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );

		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: clearing!");
	}

	void FramebufferOpenGL::setSpecificationSize(float width, float height) {
		m_specification.width = width;
		m_specification.height = height;
	}

	// ---- PRIVATE METHODS ---- //

	void FramebufferOpenGL::createColorAttachment() {
		PLATFORM_GL_FUNC(glGenTextures(1, &m_colorAttachment));
		PLATFORM_GL_FUNC(glBindTexture(GL_TEXTURE_2D, m_colorAttachment));
		PLATFORM_GL_FUNC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (size_t)m_specification.width, (size_t)m_specification.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));
		PLATFORM_GL_FUNC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		PLATFORM_GL_FUNC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: created color attachment {}", m_colorAttachment);
	}

	void FramebufferOpenGL::createDepthAttachment() {
		PLATFORM_GL_FUNC(glGenRenderbuffers(1, &m_depthAttanchment));
		PLATFORM_GL_FUNC(glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttanchment));
		PLATFORM_GL_FUNC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (size_t)m_specification.width, (size_t)m_specification.height));
		PLATFORM_GL_FUNC(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: created depth attachment {}", m_depthAttanchment);
	}

	void FramebufferOpenGL::createFramebuffer() {
		PLATFORM_GL_FUNC(glGenFramebuffers(1, &m_id));
		PLATFORM_GL_FUNC(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
		PLATFORM_GL_FUNC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0));
		PLATFORM_GL_FUNC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttanchment));

		PLATFORM_TRACE("FRAMEBUFFER_OPENGL: created framebuffer {} with color {} depth {}", m_id, m_colorAttachment, m_depthAttanchment);
	}

	void FramebufferOpenGL::checkCreationStatus() {
		auto framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
			PLATFORM_ERROR("FRAMEBUFFER_OPENGL: Cannot load framebuffer!");
		}
		else {
			PLATFORM_INFO("FRAMEBUFFER_OPENGL: loaded successfully - {}!", m_id);
		}
	}


}
