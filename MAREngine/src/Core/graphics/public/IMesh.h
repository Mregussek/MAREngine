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


#ifndef MARENGINE_IMESH_H
#define MARENGINE_IMESH_H


#include "IRenderResource.h"


namespace marengine {

    struct CRenderable;
    class FMeshProxy;
    class FMeshStorage;


    static constexpr int8 g_MeshDefaultTypeIndex{ 127 };

    struct FMeshExternalInfo {
        std::string path{};
    };

    enum class EMeshType {
        NONE, CUBE, PYRAMID, SURFACE, EXTERNAL
    };


    class IMeshProxy : public FRenderResource {
    public:

        virtual const FVertexArray& getVertices() const = 0;
        virtual const FIndicesArray& getIndices() const = 0;
        virtual EMeshType getType() const = 0;
        virtual const char* getName() const = 0;

    };


    class IMeshStorage : public IRenderResourceStorage {
    public:

        virtual const FMeshProxy* getExternal(int32 index) const = 0;
        virtual uint32 getCountExternal() const = 0;

        virtual const FMeshProxy* getCube() const = 0;
        virtual const FMeshProxy* getPyramid() const = 0;
        virtual const FMeshProxy* getSurface() const = 0;

        virtual const FMeshProxy* retrieve(const CRenderable& cRenderable) const = 0;
        virtual const FMeshProxy* retrieve(const char* name) const = 0;

        virtual const FMeshProxy* isAlreadyLoaded(const CRenderable& cRenderable) const = 0;

    };


    class IMeshFactory : public FRenderResourceFactory {
    public:

        virtual FMeshProxy* emplaceExternal(const std::string& path) = 0;
        virtual FMeshStorage* getStorage() const = 0;

    };



}


#endif //MARENGINE_IMESH_H
