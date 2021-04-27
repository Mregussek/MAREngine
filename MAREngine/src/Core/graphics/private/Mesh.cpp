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


#include "../public/Mesh.h"
#include "loader_obj/OBJ_Loader.h"
#include "../../ecs/Entity/Components.h"
#include "../../../ProjectManager.h"
#include "../../../Logging/Logger.h"


namespace marengine {


    const FVertexArray& FMeshProxy::getVertices() const {
        return p_vertices;
    }

    const FIndicesArray& FMeshProxy::getIndices() const {
        return p_indices;
    }

    EMeshType FMeshProxy::getType() const {
        return p_type;
    }


    FMeshExternal::FMeshExternal() {
        p_type = EMeshType::EXTERNAL;
    }

    void FMeshExternal::load(const std::string& path) {
        p_info.path = path;

        loader_obj::Loader Loader{};
        const bool correctlyLoaded{ Loader.LoadFile(path) };
        if (!correctlyLoaded) {
            MARLOG_ERR(ELoggerType::GRAPHICS, "Could not load mesh -> {}", path);
            return;
        }

        if (Loader.LoadedMeshes.size() == 1) {
            p_vertices = Loader.LoadedMeshes[0].Vertices;
            p_indices = Loader.LoadedMeshes[0].Indices;
        }
        else {
            MARLOG_ERR(ELoggerType::GRAPHICS, "Loaded Meshes > 1 are not supported!");
        }
    }

    const FMeshExternalInfo& FMeshExternal::getInfo() const {
        return p_info;
    }


    const FMeshProxy* FMeshStorage::getExternal(int32 index) const {
        if(index < 0) {
            MARLOG_ERR(ELoggerType::GRAPHICS, "Given wrong index -> {}", index);
            return nullptr;
        }
        return &m_externalArray.at(index);
    }

    uint32 FMeshStorage::getCountExternal() const {
        return m_externalArray.size();
    }

    const FMeshProxy* FMeshStorage::getCube() const {
        return &m_cube;
    }

    const FMeshProxy* FMeshStorage::getPyramid() const {
        return &m_pyramid;
    }

    const FMeshProxy* FMeshStorage::getSurface() const {
        return &m_surface;
    }

    const FMeshProxy* FMeshStorage::retrieve(const CRenderable& renderable) const {
        switch(renderable.mesh.type) {
            case EMeshType::EXTERNAL: return getExternal(renderable.mesh.index);
            case EMeshType::CUBE: return getCube();
            case EMeshType::PYRAMID: return getPyramid();
            case EMeshType::SURFACE: return getSurface();
            default: return nullptr;
        }
    }

    const FMeshProxy* FMeshStorage::retrieve(const char* name) const {
        if(std::strcmp(getCube()->getName(), name) == 0) {
            return getCube();
        }
        if(std::strcmp(getPyramid()->getName(), name) == 0) {
            return getPyramid();
        }
        if(std::strcmp(getSurface()->getName(), name) == 0) {
            return getSurface();
        }

        for(uint32 i = 0; i < getCountExternal(); i++) {
            if(std::strcmp(getExternal(i)->getName(), name) == 0) {
                return getExternal(i);
            }
        }

        return nullptr;
    }

    const FMeshProxy* FMeshStorage::isAlreadyLoaded(const CRenderable& cRenderable) const {
        const auto fromBegin{ m_externalArray.cbegin() };
        const auto toEnd{ m_externalArray.cend() };
        auto alreadyLoaded = [&path = std::as_const(cRenderable.mesh.path)](const FMeshExternal& mesh)->bool {
            return path == mesh.getInfo().path;
        };
        const auto it = std::find_if(fromBegin, toEnd, alreadyLoaded);
        if(it != toEnd) { // found already loaded mesh
            return (FMeshProxy*)&(*it);
        }

        // If no mesh proxy found
        return nullptr;
    }

    void FMeshStorage::reset() {
        m_externalArray.clear();
    }


    FMeshProxy* FMeshFactory::emplaceExternal(const std::string& path) {
        auto& mesh{ m_storage.m_externalArray.emplace_back() };
        const int8 currentSize{ (int8)m_storage.getCountExternal() };
        mesh.setIndex( currentSize - 1);
        mesh.load(FProjectManager::getAssetsPath() + path);
        return &mesh;
    }

    FMeshStorage* FMeshFactory::getStorage() const {
        return const_cast<FMeshStorage*>(&m_storage);
    }


    FMeshCube::FMeshCube() {
        p_vertices = {
                //  front (x, y, z)		    // LightNormal			    // Texture		    // ShapeIndex
                { { -1.0f, -1.0f,  1.0f },	{ -1.0f, -1.0f,  2.0f },	{ 0.0f, 0.0f },		0.0f }, // 0
                { {  1.0f, -1.0f,  1.0f },	{  2.0f, -2.0f,  1.0f },	{ 1.0f, 0.0f },		0.0f }, // 1
                { {  1.0f,  1.0f,  1.0f },	{  1.0f,  1.0f,  2.0f },	{ 1.0f, 1.0f },		0.0f }, // 2
                { { -1.0f,  1.0f,  1.0f },	{ -2.0f,  2.0f,  1.0f },	{ 0.0f, 1.0f },		0.0f }, // 3
                //  back
                { { -1.0f, -1.0f, -1.0f },	{ -2.0f, -2.0f, -1.0f },	{ 0.0f, 0.0f },		0.0f }, // 4
                { {  1.0f, -1.0f, -1.0f },	{  1.0f, -1.0f, -2.0f },	{ 1.0f, 0.0f },		0.0f }, // 5
                { {  1.0f,  1.0f, -1.0f },	{  2.0f,  2.0f, -1.0f },	{ 1.0f, 1.0f },		0.0f }, // 6
                { { -1.0f,  1.0f, -1.0f },	{ -1.0f,  1.0f, -2.0f },	{ 0.0f, 1.0f },		0.0f }  // 7
        };
        p_indices = {
                // front	// back
                0, 1, 2,	7, 6, 5,
                2, 3, 0,	5, 4, 7,
                // right	// left
                1, 5, 6,	4, 0, 3,
                6, 2, 1,	3, 7, 4,
                // bottom	// top
                4, 5, 1,	3, 2, 6,
                1, 0, 4,	6, 7, 3
        };
        p_type = EMeshType::CUBE;
    }

    const char* FMeshCube::getName() const {
        return "Cube";
    }


    FMeshPyramid::FMeshPyramid() {
        p_vertices = {
                // (x, y, z)			    // LightNormal			                // TextureCoords     // ShapeIndex
                { {-1.0f, -1.0f,  1.0f},	{-0.894427f, 2.89443f, 1.89443f  },		{ 0.0f, 0.0f },		 0.0f },
                { { 1.0f, -1.0f,  1.0f},	{ 0.894427f, 1.89443f, 1.89443f  },		{ 0.0f, 1.0f },		 0.0f },
                { { 1.0f, -1.0f, -1.0f},	{ 0.894427f, 2.89443f, 0.105573f },	    { 0.0f, 0.0f },		 0.0f },
                { {-1.0f, -1.0f, -1.0f},	{-0.894427f, 1.89443f, 0.105573f },	    { 0.0f, 1.0f },		 0.0f },
                { { 0.0f,  1.0f,  0.0f},	{ 0.f,		1.78885f, 1.f        },		{ 0.5f, 0.5f },		 0.0f }
        };
        p_indices = {
                0, 1, 2,	2, 3, 0, // fundamental quad
                0, 1, 4,	1, 2, 4, // side triangles
                2, 3, 4,	3, 0, 4
        };
        p_type = EMeshType::PYRAMID;
    }

    const char* FMeshPyramid::getName() const {
        return "Pyramid";
    }


    FMeshSurface::FMeshSurface() {
        p_vertices = {
                // (x, y, z)			        // LightNormal			// TextureCoords	    // ShapeIndex
                { { -15.0f, -1.0f,  15.0f },	{ 0.f, 2.f, 1.f },		{ 0.0f, 0.0f },			0.0f }, // 0
                { {  15.0f, -1.0f,  15.0f },	{ 0.f, 1.f, 1.f },		{ 0.0f, 1.0f },			0.0f }, // 1
                { {  15.0f, -1.0f, -15.0f },	{ 0.f, 2.f, 1.f },		{ 1.0f, 1.0f },			0.0f }, // 2
                { { -15.0f, -1.0f, -15.0f },	{ 0.f, 1.f, 1.f },		{ 1.0f, 0.0f },			0.0f }  // 3
        };
        p_indices = {
                0, 1, 2, // first triangle
                2, 3, 0  // second triangle
        };
        p_type = EMeshType::SURFACE;
    }

    const char* FMeshSurface::getName() const {
        return "Surface";
    }


}