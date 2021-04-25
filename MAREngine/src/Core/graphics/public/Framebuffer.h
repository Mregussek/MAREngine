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


#ifndef MARENGINE_FRAMEBUFFER_H
#define MARENGINE_FRAMEBUFFER_H


#include "IFramebuffer.h"


namespace marengine {


    class FFramebuffer : public IFramebuffer {
    public:

        void setSize(uint32 width, uint32 height) final;
        void setClearColor(maths::vec3 clearColor) final;

        MAR_NO_DISCARD uint32 getWidth() const final;
        MAR_NO_DISCARD uint32 getHeight() const final;

    protected:

        FFramebufferSpecification p_specification;

    };


    class FFramebufferStorage : public IFramebufferStorage {

    };


    class FFramebufferFactory : public IFramebufferFactory {

    };


}


#endif //MARENGINE_FRAMEBUFFER_H
