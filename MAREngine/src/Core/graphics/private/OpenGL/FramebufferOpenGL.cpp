/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "FramebufferOpenGL.h"
#include "../../../../Logging/Logger.h"


namespace marengine {


    static void createColorAttachmentGL(uint32& id, const FFramebufferSpecification& specs) {
        GL_FUNC( glGenTextures(1, &id) );
        GL_FUNC( glBindTexture(GL_TEXTURE_2D, id) );
        GL_FUNC( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                              specs.width, specs.height, 0, GL_RGB, GL_FLOAT, 0) );
        GL_FUNC( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        GL_FUNC( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    }

    static void createDepthAttachmentGL(uint32& id, const FFramebufferSpecification& specs) {
        GL_FUNC( glGenRenderbuffers(1, &id) );
        GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, id) );
        GL_FUNC( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                       specs.width, specs.height) );
        GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, 0) );
    }

    static void createFramebufferGL(uint32& id, uint32 color, uint32 depth,
                                    const FFramebufferSpecification& specs) {
        GL_FUNC( glGenFramebuffers(1, &id) );
        GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, id) );
        GL_FUNC( glFramebufferTexture2D(GL_FRAMEBUFFER,
                                        GL_COLOR_ATTACHMENT0,
                                        GL_TEXTURE_2D,
                                        color,
                                        0));
        GL_FUNC( glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                           GL_DEPTH_ATTACHMENT,
                                           GL_RENDERBUFFER,
                                           depth));
    }

    static void checkFramebufferStatusGL() {
        const auto isFramebufferOk{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
        if(isFramebufferOk == GL_FALSE) {
            MARLOG_ERR(ELoggerType::PLATFORMS, "Framebuffer could not be created...");
        }
    }

    static void destroyFramebufferGL(uint32& id, uint32& color, uint32& depth) {
        GL_FUNC( glDeleteFramebuffers(1, &id) );
        GL_FUNC( glDeleteTextures(1, &color) );
        GL_FUNC( glDeleteTextures(1, &depth) );
    }

    static void bindFramebufferGL(uint32 id) {
        GL_FUNC( glBindFramebuffer(GL_FRAMEBUFFER, id) );
    }

    static void clearGL(uint32 id, const FFramebufferSpecification& specs) {
        bindFramebufferGL(id);
        GL_FUNC( glViewport(0, 0, specs.width, specs.height) );
        GL_FUNC( glClearColor(specs.clearColor.x, specs.clearColor.y, specs.clearColor.z, 1.0f) );
        GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
    }

    static void bindTextureGL(uint32 id) {
        GL_FUNC( glBindTexture(GL_TEXTURE_2D, id) );
    }

    static void bindRenderBufferGL(uint32 id) {
        GL_FUNC( glBindRenderbuffer(GL_RENDERBUFFER, id) );
    }

    static void resizeGL(uint32 color, uint32 depth, const FFramebufferSpecification& specs) {
        bindTextureGL(color);
        GL_FUNC( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                              specs.width, specs.height, 0, GL_RGB, GL_FLOAT, 0) );
        bindTextureGL(0);
        bindRenderBufferGL(depth);
        GL_FUNC( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                                       specs.width, specs.height) );
        bindRenderBufferGL(0);
    }


    void FFramebufferOpenGL::create(const FFramebufferSpecification& specs) {
        p_specification = specs;

        createColorAttachmentGL(m_colorAttachment, specs);
        createDepthAttachmentGL(m_depthAttachment, specs);
        createFramebufferGL(m_id, m_colorAttachment, m_depthAttachment, specs);
        checkFramebufferStatusGL();
        bindFramebufferGL(0);
    }

    void FFramebufferOpenGL::destroy() {
        destroyFramebufferGL(m_id, m_colorAttachment, m_depthAttachment);
    }

    void FFramebufferOpenGL::bind() const {
        bindFramebufferGL(m_id);
    }

    void FFramebufferOpenGL::unbind() const {
        bindFramebufferGL(0);
    }

    void FFramebufferOpenGL::clear() const {
        clearGL(m_id, p_specification);
    }

    void FFramebufferOpenGL::resize(uint32 width, uint32 height) {
        setSize(width, height);
        resizeGL(m_colorAttachment, m_depthAttachment, p_specification);
    }

    uint32 FFramebufferOpenGL::getColorAttach() const {
        return m_colorAttachment;
    }


    FFramebuffer* FFramebufferStorageOpenGL::get(int32 index) const {
        return (FFramebuffer*) &m_framebuffers.at(index);
    }

    uint32 FFramebufferStorageOpenGL::getCount() const {
        return m_framebuffers.size();
    }

    void FFramebufferStorageOpenGL::reset() {
        for(auto& framebuffer : m_framebuffers) {
            framebuffer.destroy();
        }
        m_framebuffers.clear();
    }


    template<typename TReturnType, typename TArray>
    static TReturnType* emplaceBufferAtArray(TArray& array) {
        auto& variable{ array.emplace_back() };
        const auto currentSize{ (int32)array.size() };
        variable.setIndex(currentSize - 1);
        return (TReturnType*)&variable;
    }

    FFramebuffer* FFramebufferFactoryOpenGL::emplace() {
        return emplaceBufferAtArray<FFramebuffer>(m_storage.m_framebuffers);
    }


}
