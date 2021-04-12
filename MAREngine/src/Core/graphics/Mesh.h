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


#ifndef MARENGINE_MESH_H
#define MARENGINE_MESH_H


#include "IRender.h"


namespace marengine {

    struct CRenderable;

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

    };


    class FMeshProxy : public IMeshProxy {
    public:

        MAR_NO_DISCARD const FVertexArray& getVertices() const final;
        MAR_NO_DISCARD const FIndicesArray& getIndices() const final;
        MAR_NO_DISCARD EMeshType getType() const final;

    protected:

        FVertexArray p_vertices;
        FIndicesArray p_indices;
        EMeshType p_type{ EMeshType::NONE };

    };


    class FMeshExternal : public FMeshProxy {
    public:

        FMeshExternal();
        void load(const std::string& path);

        MAR_NO_DISCARD const FMeshExternalInfo& getInfo() const;

    protected:

        FMeshExternalInfo p_info;

    };

    typedef std::vector<FMeshExternal> FMeshExternalArray;


    class FMeshCube : public FMeshProxy {
    public: FMeshCube();
    };


    class FMeshPyramid : public FMeshProxy {
    public: FMeshPyramid();
    };


    class FMeshSurface : public FMeshProxy {
    public: FMeshSurface();
    };


    class IMeshStorage : public IRenderResourceStorage {
    public:

        virtual const FMeshProxy* getExternal(int8 index) const = 0;
        virtual size_t getCountExternal() const = 0;

        virtual const FMeshProxy* getCube() const = 0;
        virtual const FMeshProxy* getPyramid() const = 0;
        virtual const FMeshProxy* getSurface() const = 0;

        virtual const FMeshProxy* retrieve(const CRenderable& cRenderable) const = 0;

        virtual const FMeshProxy* isAlreadyLoaded(const CRenderable& cRenderable) const = 0;

    };


    class FMeshStorage : public IMeshStorage {

        friend class FMeshFactory;

    public:

        MAR_NO_DISCARD const FMeshProxy* getExternal(int8 index) const final;
        MAR_NO_DISCARD size_t getCountExternal() const final;

        MAR_NO_DISCARD const FMeshProxy* getCube() const final;
        MAR_NO_DISCARD const FMeshProxy* getPyramid() const final;
        MAR_NO_DISCARD const FMeshProxy* getSurface() const final;

        MAR_NO_DISCARD const FMeshProxy* retrieve(const CRenderable& cRenderable) const final;

        MAR_NO_DISCARD const FMeshProxy* isAlreadyLoaded(const CRenderable& cRenderable) const final;

        void reset() final;

    private:

        FMeshExternalArray m_externalArray;
        FMeshCube m_cube;
        FMeshPyramid m_pyramid;
        FMeshSurface m_surface;

    };


    class IMeshFactory : public IRenderResourceFactory {
    public:

        virtual FMeshProxy* emplaceExternal(const std::string& path) = 0;

    };


    class FMeshFactory : public IMeshFactory {
    public:

        MAR_NO_DISCARD FMeshProxy* emplaceExternal(const std::string& path) final;

        MAR_NO_DISCARD FMeshStorage* getStorage() const;

    private:

        FMeshStorage m_storage;

    };



}


#endif //MARENGINE_MESH_H
