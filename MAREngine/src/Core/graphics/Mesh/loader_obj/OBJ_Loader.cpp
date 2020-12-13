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


#include "OBJ_Loader.h"


namespace mar::graphics::loader_obj {


	// -------------------------------------
	// MESH
	// -------------------------------------

	Mesh::Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices) {
		Vertices = _Vertices;
		Indices = _Indices;
	}

	// -------------------------------------
	// namespace ALGORITHM
	// -------------------------------------

	namespace algorithm {
		// Split a String into a string array at a given token
		inline void split(const std::string& in, std::vector<std::string>& out, std::string token) {
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string& in)
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string& in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}
	}

	Loader::~Loader() {
		LoadedMeshes.clear();
	}

	// Load a file into the loader
	//
	// If file is loaded return true
	//
	// If the file is unable to be found
	// or unable to be loaded return false
	bool Loader::LoadFile(std::string Path) {
		if (Path.substr(Path.size() - 4, 4) != ".obj") { return false; }

		std::ifstream file(Path);

		if (!file.is_open()) { return false; }

		LoadedMeshes.clear();
		LoadedVertices.clear();
		LoadedIndices.clear();

		std::vector<Vector3> Positions;
		std::vector<Vector2> TCoords;
		std::vector<Vector3> Normals;

		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;

		std::vector<std::string> MeshMatNames;

		bool listening = false;
		std::string meshname;

		Mesh tempMesh;

		std::string curline;

		while (std::getline(file, curline)) {
			const bool itIsObjectName_o{ algorithm::firstToken(curline) == "o" };
			const bool itIsPolygonGroup_g{ algorithm::firstToken(curline) == "g" };
			const bool firstLetterIsG{ curline[0] == 'g' };
			const bool firstLetterDefinesObject{ itIsObjectName_o || itIsPolygonGroup_g };

			const bool itIsVertexPosition{ algorithm::firstToken(curline) == "v" };
			const bool itIsVertexNormal{ algorithm::firstToken(curline) == "vn" };
			const bool itIsTextureCoordinate{ algorithm::firstToken(curline) == "vt" };
			const bool itIsFace{ algorithm::firstToken(curline) == "f" };
			const bool itIsMaterial{ algorithm::firstToken(curline) == "usemtl" };
			const bool itIsMaterialLibrary{ algorithm::firstToken(curline) == "mtllib" };

			// Generate a Mesh Object or Prepare for an object to be created
			if (firstLetterDefinesObject || firstLetterIsG) {
				if (!listening) {
					listening = true;

					if (firstLetterDefinesObject) { meshname = algorithm::tail(curline); }
					else { meshname = "unnamed"; }
				}
				else {
					if (!Indices.empty() && !Vertices.empty()) {
						// Create Mesh
						tempMesh = Mesh(Vertices, Indices);
						tempMesh.MeshName = meshname;

						// Insert Mesh
						LoadedMeshes.push_back(tempMesh);

						// Cleanup
						Vertices.clear();
						Indices.clear();
						meshname.clear();

						meshname = algorithm::tail(curline);
					}
					else {
						if (firstLetterDefinesObject) { meshname = algorithm::tail(curline); }
						else { meshname = "unnamed"; }
					}
				}
			}
			else if(itIsVertexPosition) {
				std::vector<std::string> spos;
				algorithm::split(algorithm::tail(curline), spos, " ");
				Positions.emplace_back(std::stof(spos[0]), std::stof(spos[1]), std::stof(spos[2]));
			}
			else if(itIsTextureCoordinate) {
				std::vector<std::string> stex;
				algorithm::split(algorithm::tail(curline), stex, " ");
				TCoords.emplace_back(std::stof(stex[0]), std::stof(stex[1]));
			}
			else  if (itIsVertexNormal) {
				std::vector<std::string> snor;
				algorithm::split(algorithm::tail(curline), snor, " ");
				Normals.emplace_back(std::stof(snor[0]), std::stof(snor[1]), std::stof(snor[2]));
			}
			else if (itIsFace) {
				std::vector<Vertex> vVerts;
				GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

				for (const auto& vertex : vVerts) {
					Vertices.push_back(vertex);
					LoadedVertices.push_back(vertex);
				}

				std::vector<uint32_t> iIndices;
				VertexTriangluation(iIndices, vVerts);

				for (const auto& iIndice : iIndices) {
					const auto meshIndNum{ (uint32_t)((Vertices.size()) - vVerts.size()) + iIndice };
					Indices.push_back(meshIndNum);

					const auto loaderIndNum{ (uint32_t)((LoadedVertices.size()) - vVerts.size()) + iIndice };
					LoadedIndices.push_back(loaderIndNum);
				}
			}
			else if(itIsMaterial) {
				MeshMatNames.push_back(algorithm::tail(curline));

				if (!Indices.empty() && !Vertices.empty()) {
					tempMesh = Mesh(Vertices, Indices);
					tempMesh.MeshName = meshname;
					const int32_t i{ 2 };
					while (1) {
						tempMesh.MeshName = meshname + "_" + std::to_string(i);

						for (auto& m : LoadedMeshes) {
							if (m.MeshName == tempMesh.MeshName) { continue; }
						}
							
						break;
					}

					LoadedMeshes.push_back(tempMesh);

					Vertices.clear();
					Indices.clear();
				}
			}
			else if (itIsMaterialLibrary) {
				std::vector<std::string> temp;
				algorithm::split(Path, temp, "/");

				std::string pathtomat = "";

				if (temp.size() != 1) {
					for (size_t i = 0; i < temp.size() - 1; i++) {
						pathtomat += temp[i] + "/";
					}
				}

				pathtomat += algorithm::tail(curline);

				LoadMaterials(pathtomat);
			}
		}

		// Deal with last mesh
		if (!Indices.empty() && !Vertices.empty()) {
			tempMesh = Mesh(Vertices, Indices);
			tempMesh.MeshName = meshname;

			LoadedMeshes.push_back(tempMesh);
		}

		file.close();

		// Set Materials for each Mesh
		for (size_t i = 0; i < MeshMatNames.size(); i++) {
			std::string matname = MeshMatNames[i];

			// Find corresponding material name in loaded materials
			// when found copy material variables into mesh material
			for (size_t j = 0; j < LoadedMaterials.size(); j++) {
				if (LoadedMaterials[j].name == matname) {
					LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
					break;
				}
			}
		}

		const bool couldNotLoadAnything{ LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty() };

		if (couldNotLoadAnything) { return false; }
		else { return true; }
	}

	// Generate vertices from a list of positions, 
	//	tcoords, normals and a face line
	void Loader::GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
		const std::vector<Vector3>& iPositions,
		const std::vector<Vector2>& iTCoords,
		const std::vector<Vector3>& iNormals,
		std::string icurline)
	{
		std::vector<std::string> sface, svert;
		Vertex vVert;
		algorithm::split(algorithm::tail(icurline), sface, " ");

		bool noNormal = false;

		// For every given vertex do this
		for (int i = 0; i < int(sface.size()); i++)
		{
			// See What type the vertex is.
			int vtype = 0;

			algorithm::split(sface[i], svert, "/");

			// Check for just position - v1
			if (svert.size() == 1)
			{
				// Only position
				vtype = 1;
			}

			// Check for position & texture - v1/vt1
			if (svert.size() == 2)
			{
				// Position & Texture
				vtype = 2;
			}

			// Check for Position, Texture and Normal - v1/vt1/vn1
			// or if Position and Normal - v1//vn1
			if (svert.size() == 3)
			{
				if (svert[1] != "")
				{
					// Position, Texture, and Normal
					vtype = 4;
				}
				else
				{
					// Position & Normal
					vtype = 3;
				}
			}

			// Calculate and store the vertex
			switch (vtype)
			{
			case 1: // P
			{
				vVert.position = algorithm::getElement(iPositions, svert[0]);
				vVert.textureCoordinates = Vector2(0, 0);
				noNormal = true;
				oVerts.push_back(vVert);
				break;
			}
			case 2: // P/T
			{
				vVert.position = algorithm::getElement(iPositions, svert[0]);
				vVert.textureCoordinates = algorithm::getElement(iTCoords, svert[1]);
				noNormal = true;
				oVerts.push_back(vVert);
				break;
			}
			case 3: // P//N
			{
				vVert.position = algorithm::getElement(iPositions, svert[0]);
				vVert.textureCoordinates = Vector2(0, 0);
				vVert.lightNormal = algorithm::getElement(iNormals, svert[2]);
				oVerts.push_back(vVert);
				break;
			}
			case 4: // P/T/N
			{
				vVert.position = algorithm::getElement(iPositions, svert[0]);
				vVert.textureCoordinates = algorithm::getElement(iTCoords, svert[1]);
				vVert.lightNormal = algorithm::getElement(iNormals, svert[2]);
				oVerts.push_back(vVert);
				break;
			}
			default:
			{
				break;
			}
			}
		}

		// take care of missing normals
		// these may not be truly acurate but it is the 
		// best they get for not compiling a mesh with normals	
		if (noNormal) {
			const Vector3 A{ oVerts[0].position - oVerts[1].position };
			const Vector3 B{ oVerts[2].position - oVerts[1].position };
			const Vector3 normal{ Vector3::cross(A, B) };

			for (auto& oVertex : oVerts) {
				oVertex.lightNormal = normal;
			}
		}
	}

	// Triangulate a list of vertices into a face by printing
	//	indicies corresponding with triangles within it
	void Loader::VertexTriangluation(std::vector<unsigned int>& oIndices,
		const std::vector<Vertex>& iVerts)
	{
		// If there are 2 or less verts,
		// no triangle can be created,
		// so exit
		if (iVerts.size() < 3) {
			return;
		}
		// If it is a triangle no need to calculate it
		if (iVerts.size() == 3) {
			oIndices.push_back(0);
			oIndices.push_back(1);
			oIndices.push_back(2);
			return;
		}

		// Create a list of vertices
		std::vector<Vertex> tVerts = iVerts;

		while (true)
		{
			// For every vertex
			for (int i = 0; i < int(tVerts.size()); i++)
			{
				// pPrev = the previous vertex in the list
				Vertex pPrev;
				if (i == 0)
				{
					pPrev = tVerts[tVerts.size() - 1];
				}
				else
				{
					pPrev = tVerts[i - 1];
				}

				// pCur = the current vertex;
				Vertex pCur = tVerts[i];

				// pNext = the next vertex in the list
				Vertex pNext;
				if (i == tVerts.size() - 1)
				{
					pNext = tVerts[0];
				}
				else
				{
					pNext = tVerts[i + 1];
				}

				// Check to see if there are only 3 verts left
				// if so this is the last triangle
				if (tVerts.size() == 3)
				{
					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(tVerts.size()); j++)
					{
						if (iVerts[j].position == pCur.position)
							oIndices.push_back(j);
						if (iVerts[j].position == pPrev.position)
							oIndices.push_back(j);
						if (iVerts[j].position == pNext.position)
							oIndices.push_back(j);
					}

					tVerts.clear();
					break;
				}
				if (tVerts.size() == 4)
				{
					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j].position == pCur.position)
							oIndices.push_back(j);
						if (iVerts[j].position == pPrev.position)
							oIndices.push_back(j);
						if (iVerts[j].position == pNext.position)
							oIndices.push_back(j);
					}

					Vector3 tempVec;
					for (int j = 0; j < int(tVerts.size()); j++)
					{
						if (tVerts[j].position != pCur.position
							&& tVerts[j].position != pPrev.position
							&& tVerts[j].position != pNext.position)
						{
							tempVec = tVerts[j].position;
							break;
						}
					}

					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j].position == pPrev.position)
							oIndices.push_back(j);
						if (iVerts[j].position == pNext.position)
							oIndices.push_back(j);
						if (iVerts[j].position == tempVec)
							oIndices.push_back(j);
					}

					tVerts.clear();
					break;
				}

				// If Vertex is not an interior vertex
				float angle = Vector3::angleBetween(pPrev.position - pCur.position, pNext.position - pCur.position) * (180.f / (float)3.14159265359);
				if (angle <= 0.f && angle >= 180.f)
					continue;

				// If any vertices are within this triangle
				bool inTri = false;
				for (int j = 0; j < int(iVerts.size()); j++)
				{
					if (Vector3::inTriangle(iVerts[j].position, pPrev.position, pCur.position, pNext.position)
						&& iVerts[j].position != pPrev.position
						&& iVerts[j].position != pCur.position
						&& iVerts[j].position != pNext.position)
					{
						inTri = true;
						break;
					}
				}
				if (inTri)
					continue;

				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(iVerts.size()); j++)
				{
					if (iVerts[j].position == pCur.position)
						oIndices.push_back(j);
					if (iVerts[j].position == pPrev.position)
						oIndices.push_back(j);
					if (iVerts[j].position == pNext.position)
						oIndices.push_back(j);
				}

				// Delete pCur from the list
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].position == pCur.position)
					{
						tVerts.erase(tVerts.begin() + j);
						break;
					}
				}

				// reset i to the start
				// -1 since loop will add 1 to it
				i = -1;
			}

			// if no triangles were created
			if (oIndices.size() == 0)
				break;

			// if no more vertices
			if (tVerts.size() == 0)
				break;
		}
	}

	// Load Materials from .mtl file
	bool Loader::LoadMaterials(std::string path)
	{
		// If the file is not a material file return false
		if (path.substr(path.size() - 4, path.size()) != ".mtl")
			return false;

		std::ifstream file(path);

		// If the file is not found return false
		if (!file.is_open())
			return false;

		Material tempMaterial;

		bool listening = false;

		// Go through each line looking for material variables
		std::string curline;
		while (std::getline(file, curline))
		{
			// new material and material name
			if (algorithm::firstToken(curline) == "newmtl")
			{
				if (!listening)
				{
					listening = true;

					if (curline.size() > 7)
					{
						tempMaterial.name = algorithm::tail(curline);
					}
					else
					{
						tempMaterial.name = "none";
					}
				}
				else
				{
					// Generate the material

					// Push Back loaded Material
					LoadedMaterials.push_back(tempMaterial);

					// Clear Loaded Material
					tempMaterial = Material();

					if (curline.size() > 7)
					{
						tempMaterial.name = algorithm::tail(curline);
					}
					else
					{
						tempMaterial.name = "none";
					}
				}
			}
			// Ambient Color
			if (algorithm::firstToken(curline) == "Ka")
			{
				std::vector<std::string> temp;
				algorithm::split(algorithm::tail(curline), temp, " ");

				if (temp.size() != 3)
					continue;

				tempMaterial.Ka.x = std::stof(temp[0]);
				tempMaterial.Ka.y = std::stof(temp[1]);
				tempMaterial.Ka.z = std::stof(temp[2]);
			}
			// Diffuse Color
			if (algorithm::firstToken(curline) == "Kd")
			{
				std::vector<std::string> temp;
				algorithm::split(algorithm::tail(curline), temp, " ");

				if (temp.size() != 3)
					continue;

				tempMaterial.Kd.x = std::stof(temp[0]);
				tempMaterial.Kd.y = std::stof(temp[1]);
				tempMaterial.Kd.z = std::stof(temp[2]);
			}
			// Specular Color
			if (algorithm::firstToken(curline) == "Ks")
			{
				std::vector<std::string> temp;
				algorithm::split(algorithm::tail(curline), temp, " ");

				if (temp.size() != 3)
					continue;

				tempMaterial.Ks.x = std::stof(temp[0]);
				tempMaterial.Ks.y = std::stof(temp[1]);
				tempMaterial.Ks.z = std::stof(temp[2]);
			}
			// Specular Exponent
			if (algorithm::firstToken(curline) == "Ns")
			{
				tempMaterial.Ns = std::stof(algorithm::tail(curline));
			}
			// Optical Density
			if (algorithm::firstToken(curline) == "Ni")
			{
				tempMaterial.Ni = std::stof(algorithm::tail(curline));
			}
			// Dissolve
			if (algorithm::firstToken(curline) == "d")
			{
				tempMaterial.d = std::stof(algorithm::tail(curline));
			}
			// Illumination
			if (algorithm::firstToken(curline) == "illum")
			{
				tempMaterial.illum = std::stoi(algorithm::tail(curline));
			}
			// Ambient Texture Map
			if (algorithm::firstToken(curline) == "map_Ka")
			{
				tempMaterial.map_Ka = algorithm::tail(curline);
			}
			// Diffuse Texture Map
			if (algorithm::firstToken(curline) == "map_Kd")
			{
				tempMaterial.map_Kd = algorithm::tail(curline);
			}
			// Specular Texture Map
			if (algorithm::firstToken(curline) == "map_Ks")
			{
				tempMaterial.map_Ks = algorithm::tail(curline);
			}
			// Specular Hightlight Map
			if (algorithm::firstToken(curline) == "map_Ns")
			{
				tempMaterial.map_Ns = algorithm::tail(curline);
			}
			// Alpha Texture Map
			if (algorithm::firstToken(curline) == "map_d")
			{
				tempMaterial.map_d = algorithm::tail(curline);
			}
			// Bump Map
			if (algorithm::firstToken(curline) == "map_Bump" || algorithm::firstToken(curline) == "map_bump" || algorithm::firstToken(curline) == "bump")
			{
				tempMaterial.map_bump = algorithm::tail(curline);
			}
		}

		// Deal with last material

		// Push Back loaded Material
		LoadedMaterials.push_back(tempMaterial);

		// Test to see if anything was loaded
		// If not return false
		if (LoadedMaterials.empty())
			return false;
		// If so return true
		else
			return true;
	}
}