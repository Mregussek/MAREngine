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


#ifndef MARENGINE_IPIPELINE_H
#define MARENGINE_IPIPELINE_H


#include "IRenderResource.h"


namespace marengine {

    class FShadersStorage;
    class FBufferStorage;
    class FPipelineMeshColor;
    class FPipelineMeshTex2D;
    class FMeshBatchStaticColor;
    class FMeshBatchStaticTex2D;


    class IPipeline : public FRenderResource {
    public:

        virtual void passBufferStorage(FBufferStorage* pBufferStorage) = 0;
        virtual void passShadersStorage(FShadersStorage* pShadersStorage) = 0;
        virtual void passMaterialStorage(FMaterialStorage* pMaterialStorage) = 0;

        virtual void create() = 0;
        virtual void close() = 0;
        virtual void bind() const = 0;

    };


    class IPipelineStorage : public IRenderResourceStorage {
    public:

        virtual FPipelineMeshColor* getColorMesh(int32 index) const = 0;
        virtual FPipelineMeshTex2D* getTex2DMesh(int32 index) const = 0;

        virtual uint32 getCountColorMesh() const = 0;
        virtual uint32 getCountTex2DMesh() const = 0;

    };


    class IPipelineFactory : public FRenderResourceFactory {
    public:

        virtual FPipelineMeshColor* emplaceMeshColor() = 0;
        virtual FPipelineMeshTex2D* emplaceMeshTex2D() = 0;

        virtual void fillPipelineFor(FPipelineMeshColor* const pPipeline,
                                     FMeshBatchStaticColor* const pBatch) const = 0;

        virtual void fillPipelineFor(FPipelineMeshTex2D* const pPipeline,
                                     FMeshBatchStaticTex2D* const pBatch) const = 0;

    };


}


#endif //MARENGINE_IPIPELINE_H
