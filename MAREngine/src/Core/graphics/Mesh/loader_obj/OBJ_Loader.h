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


#ifndef MAR_ENGINE_GRAPHICS_MESH_LOADER_OBJ_H
#define MAR_ENGINE_GRAPHICS_MESH_LOADER_OBJ_H


#include "../../../../mar.h"
#include "../Vertex.h"


namespace marengine::loader_obj {

	typedef maths::vec2 Vector2;
	typedef maths::vec3 Vector3;


	struct Material {

		std::string name;		// Material Name
		Vector3 Ka;				// Ambient Color
		Vector3 Kd;				// Diffuse Color
		Vector3 Ks;				// Specular Color
		float Ns{ 0.f };		// Specular Exponent
		float Ni{ 0.f };		// Optical Density
		float d{ 0.f };			// Dissolve
		int32_t illum{ 0 };		// Illumination
		std::string map_Ka;		// Ambient Texture Map
		std::string map_Kd;		// Diffuse Texture Map
		std::string map_Ks;		// Specular Texture Map
		std::string map_Ns;		// Specular Hightlight Map
		std::string map_d;		// Alpha Texture Map
		std::string map_bump;	// Bump Map

	};

	// Structure: Mesh
	//
	// Description: A Simple Mesh Object that holds
	//	a name, a vertex list, and an index list
	struct Mesh {

		Mesh() = default;
		Mesh(std::vector<Vertex>& _Vertices, std::vector<uint32_t>& _Indices);

		std::string MeshName;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;

		Material MeshMaterial;
	};

	// Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm {
		// Split a String into a string array at a given token
		inline void split(const std::string& in, std::vector<std::string>& out, std::string token);

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string& in);

		// Get first token of string
		inline std::string firstToken(const std::string& in);

		// Get element at given index position
		template <class T>
		inline const T& getElement(const std::vector<T> &elements, const std::string& strIndex) {
			const auto index = [&elements, &strIndex]()->int32_t {
				const int32_t idx{ std::stoi(strIndex) };
				if (idx < 0) { 
					return (int32_t)elements.size() + idx; 
				}
				else { 
					return idx - 1;
				}
			}();

			return elements[index];
		}
	}

	// Class: Loader
	//
	// Description: The OBJ Model Loader
	class Loader {
	public:

		Loader() = default;
		~Loader();

		// Load a file into the loader
		//
		// If file is loaded return true
		//
		// If the file is unable to be found
		// or unable to be loaded return false
		bool LoadFile(std::string Path);


		std::vector<Mesh> LoadedMeshes;
		std::vector<Vertex> LoadedVertices;
		std::vector<uint32_t> LoadedIndices;
		std::vector<Material> LoadedMaterials;

	private:
		// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
		void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
			const std::vector<Vector3>& iPositions,
			const std::vector<Vector2>& iTCoords,
			const std::vector<Vector3>& iNormals,
			const std::string& icurline);

		// Triangulate a list of vertices into a face by printing
		//	inducies corresponding with triangles within it
		void VertexTriangluation(std::vector<uint32_t>& oIndices,
			const std::vector<Vertex>& iVerts);

		// Load Materials from .mtl file
		bool LoadMaterials(std::string path);


		int32_t checkVertexType(const std::string& sFace, std::vector<std::string>& sVertices) const;

	};
}


#endif // !MAR_ENGINE_GRAPHICS_MESH_LOADER_OBJ_H