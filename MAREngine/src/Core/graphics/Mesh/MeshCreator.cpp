/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "MeshCreator.h"
#include "../GraphicsLogs.h"
#include "../../ecs/Entity/EntityCollection.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace mar::graphics {


    std::vector<Vertex> MeshCreator::Cube::getVertices() {
        return {
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
    }
    
    std::vector<uint32_t> MeshCreator::Cube::getIndices() {
        return {
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
    }
    
    std::vector<Vertex> MeshCreator::Wall::getVertices() {
        return {
            //  front (x, y, z)			    // LightNormal			    // Texture		    // ShapeIndex
            { { -0.2f, -1.0f,  10.0f },		{ -1.f, -1.f,  3.f },		{ 0.0f, 0.0f },		0.0f }, // 0
            { {  0.2f, -1.0f,  10.0f },		{  2.f, -2.f,  2.f },		{ 0.0f, 0.0f },		0.0f }, // 1
            { {  0.2f,  5.0f,  10.0f },		{  1.f,  1.f,  3.f },		{ 1.0f, 0.0f },		0.0f }, // 2
            { { -0.2f,  5.0f,  10.0f },		{ -2.f,  2.f,  2.f },		{ 1.0f, 0.0f },		0.0f }, // 3
            //  back 			 					                                             
            { { -0.2f, -1.0f, -10.0f },		{ -2.f, -2.f,  0.f },		{ 0.0f, 1.0f },		0.0f }, // 4
            { {  0.2f, -1.0f, -10.0f },		{  1.f, -1.f, -1.f },		{ 0.0f, 1.0f },		0.0f }, // 5
            { {  0.2f,  5.0f, -10.0f },		{  2.f,  2.f,  0.f },		{ 1.0f, 1.0f },		0.0f }, // 6
            { { -0.2f,  5.0f, -10.0f },		{ -1.f,  1.f, -1.f },		{ 1.0f, 1.0f },		0.0f }  // 7
        };
    }
    
    std::vector<uint32_t> MeshCreator::Wall::getIndices() {
        return {
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
    }
    
    std::vector<Vertex> MeshCreator::Surface::getVertices() {
        return {
            // (x, y, z)			        // LightNormal			// TextureCoords	    // ShapeIndex
            { { -15.0f, -1.0f,  15.0f },	{ 0.f, 2.f, 1.f },		{ 0.0f, 0.0f },			0.0f }, // 0
            { {  15.0f, -1.0f,  15.0f },	{ 0.f, 1.f, 1.f },		{ 0.0f, 1.0f },			0.0f }, // 1
            { {  15.0f, -1.0f, -15.0f },	{ 0.f, 2.f, 1.f },		{ 1.0f, 1.0f },			0.0f }, // 2
            { { -15.0f, -1.0f, -15.0f },	{ 0.f, 1.f, 1.f },		{ 1.0f, 0.0f },			0.0f }  // 3
        };
    }
    
    std::vector<uint32_t> MeshCreator::Surface::getIndices() {
        return {
                0, 1, 2, // first triangle
                2, 3, 0  // second triangle
        };
    }
    
    std::vector<Vertex> MeshCreator::Pyramid::getVertices() {
        return {
            // (x, y, z)			    // LightNormal			                // TextureCoords     // ShapeIndex
            { {-1.0f, -1.0f,  1.0f},	{-0.894427f, 2.89443f, 1.89443f  },		{ 0.0f, 0.0f },		 0.0f },
            { { 1.0f, -1.0f,  1.0f},	{ 0.894427f, 1.89443f, 1.89443f  },		{ 0.0f, 1.0f },		 0.0f },
            { { 1.0f, -1.0f, -1.0f},	{ 0.894427f, 2.89443f, 0.105573f },	    { 0.0f, 0.0f },		 0.0f },
            { {-1.0f, -1.0f, -1.0f},	{-0.894427f, 1.89443f, 0.105573f },	    { 0.0f, 1.0f },		 0.0f },
            { { 0.0f,  1.0f,  0.0f},	{ 0.f,		1.78885f, 1.f        },		{ 0.5f, 0.5f },		 0.0f }
        };
    }
    
    std::vector<uint32_t> MeshCreator::Pyramid::getIndices() {
        return {
            0, 1, 2,	2, 3, 0, // fundamental quad
            0, 1, 4,	1, 2, 4, // side triangles
            2, 3, 4,	3, 0, 4
        };
    }
    
    void MeshCreator::loadOBJ(std::string filename, std::string path, const ecs::EntityCollection& collection) {
        objl::Loader Loader;
        bool loadout = Loader.LoadFile(path);
    
        if (loadout) {
            for (size_t i = 0; i < Loader.LoadedMeshes.size(); i++) {
                auto& entity = collection.createEntity();
                auto& tag = entity.getComponent<ecs::TagComponent>();
                auto& renderable = entity.addComponent<ecs::RenderableComponent>();
    
                objl::Mesh curMesh = Loader.LoadedMeshes[i];
    
                if (curMesh.MeshName.empty()) {
                    tag.tag = filename;
                    renderable.name = filename;
                }
                else {
                    tag.tag = curMesh.MeshName;
                    renderable.name = curMesh.MeshName;
                }
    
                for (size_t j = 0; j < curMesh.Vertices.size(); j++) {
                    renderable.vertices.push_back(Vertex{
                        {curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z},
                        {curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z},
                        {curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y},
                        {0.0f}
                    });
                }
    
                renderable.indices = curMesh.Indices;
            }
        }
        else {
            GRAPHICS_ERROR("MESH_CREATOR: could not load .obj file {}", path);
        }
    }


}
