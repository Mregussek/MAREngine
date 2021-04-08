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


#ifndef MARENGINE_PIPELINEOPENGL_H
#define MARENGINE_PIPELINEOPENGL_H


#include "../Pipeline.h"


namespace marengine {


    class FPipelineMeshColorOpenGL : public FPipelineMeshColor {
    public:

        void create() final;
        void close() final;
        void bind() final;

    private:

        GLuint m_vao{ 0 };

    };


    class FPipelineMeshTex2DOpenGL : public FPipelineMeshTex2D {
    public:

        void create() final;
        void close() final;
        void bind() final;

    };


    class FPipelineStorageOpenGL : public FPipelineStorage {

        friend class FPipelineFactoryOpenGL;

    public:

        MAR_NO_DISCARD FPipelineMeshColor* getColorMesh(int8_t index) const final;
        MAR_NO_DISCARD FPipelineMeshTex2D* getTex2DMesh(int8_t index) const final;

        MAR_NO_DISCARD size_t getCountColorMesh() const final;
        MAR_NO_DISCARD size_t getCountTex2DMesh() const final;

        void reset() final;

    private:

        std::vector<FPipelineMeshColorOpenGL> m_colors;
        std::vector<FPipelineMeshTex2DOpenGL> m_texs2D;

    };


    class FPipelineFactoryOpenGL : public FPipelineFactory {

        friend class FRenderContextOpenGL;

    public:

        MAR_NO_DISCARD FPipelineMeshColor* emplaceColorMesh() final;
        MAR_NO_DISCARD FPipelineMeshTex2D* emplaceTex2DMesh() final;

    private:

        FPipelineStorageOpenGL m_storage;

    };

}


#endif //MARENGINE_PIPELINEOPENGL_H
