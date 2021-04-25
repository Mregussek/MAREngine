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


#ifndef MARENGINE_IMESHBATCH_H
#define MARENGINE_IMESHBATCH_H


#include "IRenderResource.h"


namespace marengine {

    class Entity;
    class FMeshStorage;
    class FMaterialStorage;
    class FMeshBatchStaticColor;
    class FMeshBatchStaticTex2D;
    class FMeshBatchStorageStaticColor;
    class FMeshBatchStorageStaticTex2D;


    enum class EBatchType {
        NONE, MESH_STATIC_COLOR, MESH_STATIC_TEX2D
    };


    class IMeshBatch : public FRenderResource {
    public:

        virtual void reset() = 0;

        virtual const FVertexArray& getVertices() const = 0;
        virtual const FIndicesArray& getIndices() const = 0;
        virtual const FTransformsArray& getTransforms() const = 0;

        virtual void updateVertices(const Entity& entity) = 0;
        virtual void updateIndices(const Entity& entity) = 0;
        virtual void updateTransform(const Entity& entity) = 0;

        virtual bool shouldBeBatched(const Entity& entity) const = 0;
        virtual bool canBeBatched(const Entity& entity) const = 0;
        virtual void submitToBatch(const Entity& entity) = 0;

        virtual void passVBO(int32 index) = 0;
        virtual void passIBO(int32 index) = 0;
        virtual void passTransformSSBO(int32 index) = 0;

        virtual int32 getVBO() const = 0;
        virtual int32 getIBO() const = 0;
        virtual int32 getTransformSSBO() const = 0;

        virtual void passMeshStorage(FMeshStorage* pMeshStorage) = 0;
        virtual void passMaterialStorage(FMaterialStorage* pMaterialStorage) = 0;

        virtual EBatchType getType() const = 0;

    };


    class IMeshBatchStorage : public IRenderResourceStorage {
    public:

        virtual FMeshBatchStorageStaticColor* getStorageStaticColor() const = 0;
        virtual FMeshBatchStorageStaticTex2D* getStorageStaticTex2D() const = 0;

    };


    class IMeshBatchFactory : public FRenderResourceFactory {
    public:

        virtual FMeshBatchStaticColor* emplaceStaticColor() = 0;
        virtual FMeshBatchStaticTex2D* emplaceStaticTex2D() = 0;

    };


}


#endif //MARENGINE_IMESHBATCH_H
