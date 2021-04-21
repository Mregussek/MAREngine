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


#ifndef MARENGINE_IFRAMEBUFFER_H
#define MARENGINE_IFRAMEBUFFER_H


#include "IRenderResource.h"


namespace marengine {

    class FFramebuffer;


    enum class EFramebufferTextureFormat {
        NONE,
        RGBA8,
        RED_INTEGER,
        DEPTH24STENCIL8,
        DEPTH=DEPTH24STENCIL8
    };

    struct FFramebufferSpecification {
        std::vector<EFramebufferTextureFormat> textureFormats;
        maths::vec3 clearColor;
        uint32 width{ 0 };
        uint32 height{ 0 };
        uint32 samples{ 1 };
    };


    class IFramebuffer : public FRenderResource {
    public:

        virtual void create(const FFramebufferSpecification& specs) = 0;
        virtual void destroy() = 0;

        virtual void bind() const = 0;

        virtual void resize(uint32 width, uint32 height) = 0;

        virtual uint32 getColorAttach() const = 0;

        virtual void setSize(uint32 width, uint32 height) = 0;
        virtual uint32 getWidth() const = 0;
        virtual uint32 getHeight() const = 0;

    };


    class IFramebufferStorage : public IRenderResourceStorage {
    public:

        virtual FFramebuffer* get(int32 index) const = 0;
        virtual uint32 getCount() const = 0;

    };


    class IFramebufferFactory : public FRenderResourceFactory {
    public:

        virtual FFramebuffer* emplace() = 0;

    };


}


#endif //MARENGINE_IFRAMEBUFFER_H
