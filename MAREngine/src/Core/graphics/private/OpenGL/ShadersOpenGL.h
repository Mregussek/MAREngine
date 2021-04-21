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


#ifndef MARENGINE_SHADEROPENGL_H
#define MARENGINE_SHADEROPENGL_H


#include "../../public/Shaders.h"


namespace marengine {


    class FShadersOpenGL : public FShaders {
    public:

        void compile() final;
        void close() final;
        void bind() final;

    private:

        GLuint m_id{ GL_FALSE };

    };


    class FShadersStorageOpenGL : public FShadersStorage {

        friend class FShadersFactoryOpenGL;

    public:

        MAR_NO_DISCARD uint32 getCount() const final;
        MAR_NO_DISCARD FShaders* get(int32 index) const final;

        void reset() final;

    private:

        std::vector<FShadersOpenGL> m_shadersArray;

    };


    class FShadersFactoryOpenGL : public FShadersFactory {

        friend class FRenderContextOpenGL;

    public:

        MAR_NO_DISCARD FShaders* emplace() final;

    private:

        FShadersStorageOpenGL m_storage;

    };


}


#endif //MARENGINE_SHADEROPENGL_H
