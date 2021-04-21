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


#ifndef MARENGINE_FRAMEBUFFEROPENGL_H
#define MARENGINE_FRAMEBUFFEROPENGL_H


#include "../../public/Framebuffer.h"


namespace marengine {


    class FFramebufferOpenGL : public FFramebuffer {
    public:

        void create(const FFramebufferSpecification& specs) final;
        void destroy() final;

        void bind() const final;
        void unbind() const final;
        void clear() const final;

        void resize(uint32 width, uint32 height) final;

        void setSize(uint32 width, uint32 height) final;
        void setClearColor(maths::vec3 clearColor) final;

        MAR_NO_DISCARD uint32 getWidth() const final;
        MAR_NO_DISCARD uint32 getHeight() const final;
        MAR_NO_DISCARD uint32 getColorAttach() const final;

    private:

        GLuint m_id{ 0 };
        GLuint m_colorAttachment{ 0 };
        GLuint m_depthAttachment{ 0 };

    };


    class FFramebufferStorageOpenGL : public FFramebufferStorage {

        friend class FFramebufferFactoryOpenGL;

    public:

        MAR_NO_DISCARD FFramebuffer* get(int32 index) const final;
        MAR_NO_DISCARD uint32 getCount() const final;

        void reset() final;

    private:

        std::vector<FFramebufferOpenGL> m_framebuffers;

    };


    class FFramebufferFactoryOpenGL : public FFramebufferFactory {

        friend class FRenderContextOpenGL;

    public:

        MAR_NO_DISCARD FFramebuffer* emplace() final;

    private:

        FFramebufferStorageOpenGL m_storage;

    };


}



#endif //MARENGINE_FRAMEBUFFEROPENGL_H
