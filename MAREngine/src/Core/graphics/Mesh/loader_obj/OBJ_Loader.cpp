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


namespace marengine::loader_obj {


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

			const size_t inSize{ in.size() };
			for (size_t i = 0; i < inSize; i++) {
				const std::string test{ in.substr(i, token.size()) };

				if (test == token) {
					if (!temp.empty()) {
						out.push_back(temp);
						temp.clear();
						i += token.size() - 1;
					}
					else {
						out.push_back("");
					}
				}
				else if ((i + token.size()) >= in.size()) {
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else {
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string& in)
		{
			const size_t tokenStart{ in.find_first_not_of(" \t") };
			const size_t spaceStart{ in.find_first_of(" \t", tokenStart) };
			const size_t tailStart{ in.find_first_not_of(" \t", spaceStart) };
			const size_t tailEnd{ in.find_last_not_of(" \t") };
			const bool startIsNotEOF{ tailStart != std::string::npos };
			const bool endIsNotEOF{ tailEnd != std::string::npos };

			if (startIsNotEOF && endIsNotEOF) {
				return in.substr(tailStart, tailEnd - tailStart + 1);
			}
			else if (startIsNotEOF) {
				return in.substr(tailStart);
			}

			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string& in) {
			if (!in.empty()) {
				const size_t tokenStart{ in.find_first_not_of(" \t") };
				const size_t tokenEnd{ in.find_first_of(" \t", tokenStart) };
				const bool startIsNotEOF{ tokenStart != std::string::npos };
				const bool endIsNotEOF{ tokenEnd != std::string::npos };

				if (startIsNotEOF && endIsNotEOF) {
					return in.substr(tokenStart, tokenEnd - tokenStart);
				}
				else if (startIsNotEOF) {
					return in.substr(tokenStart);
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
						auto& tmpMesh = LoadedMeshes.emplace_back(Vertices, Indices);
						tmpMesh.MeshName = meshname;

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
					auto& tempMesh = LoadedMeshes.emplace_back(Vertices, Indices);
					tempMesh.MeshName = meshname;
					constexpr int32_t i{ 2 };

					while (1) {
						tempMesh.MeshName = meshname + "_" + std::to_string(i);

						for (auto& m : LoadedMeshes) {
							if (m.MeshName == tempMesh.MeshName) { continue; }
						}
							
						break;
					}

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
			auto& tempMesh = LoadedMeshes.emplace_back(Vertices, Indices);
			tempMesh.MeshName = meshname;
		}

		file.close();

		const auto meshMatNamesSize{ MeshMatNames.size() };
		for (size_t i = 0; i < meshMatNamesSize; i++) { // Set Materials for each Mesh
			const auto it = std::find_if(LoadedMaterials.cbegin(), LoadedMaterials.cend(), [&meshMatName = std::as_const(MeshMatNames[i])](const auto& material) {
				return material.name == meshMatName;
			});

			if (it != LoadedMaterials.cend()) { LoadedMeshes[i].MeshMaterial = *it; }
		}

		const bool couldNotLoadAnything{ LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty() };

		if (couldNotLoadAnything) { return false; }
		else { return true; }
	}

	int32_t Loader::checkVertexType(const std::string& sFace, std::vector<std::string>& sVertices) const {
		algorithm::split(sFace, sVertices, "/");

		if (sVertices.size() == 1) { return 1; }	// Only position
		if (sVertices.size() == 2) { return 2; }	// Position / Texture
		if (sVertices.size() == 3) {
			if (sVertices[1] != "") { return 4; }	// Position / Texture / Normal
			else { return 3; }						// Position / Normal
		}

		return 0;
	}

	// Generate vertices from a list of positions, tcoords, normals and a face line
	void Loader::GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
		const std::vector<Vector3>& iPositions,
		const std::vector<Vector2>& iTCoords,
		const std::vector<Vector3>& iNormals,
		const std::string& icurline)
	{
		std::vector<std::string> sface, svert;
		algorithm::split(algorithm::tail(icurline), sface, " ");

		bool noNormal = false;

		for (const auto& face : sface) {
			const auto vertexType{ checkVertexType(face, svert) };

			if (vertexType == 1) { // P
				auto& vertex = oVerts.emplace_back();
				vertex.position = algorithm::getElement(iPositions, svert[0]);
				vertex.textureCoordinates = Vector2(0.f, 0.f);
				noNormal = true;
			}
			else if (vertexType == 2) { // P/T
				auto& vertex = oVerts.emplace_back();
				vertex.position = algorithm::getElement(iPositions, svert[0]);
				vertex.textureCoordinates = algorithm::getElement(iTCoords, svert[1]);
				noNormal = true;
			}
			else if (vertexType == 3) { // P//N
				auto& vertex = oVerts.emplace_back();
				vertex.position = algorithm::getElement(iPositions, svert[0]);
				vertex.textureCoordinates = Vector2(0.f, 0.f);
				vertex.lightNormal = algorithm::getElement(iNormals, svert[2]);
			}
			else if (vertexType == 4) { // P/T/N
				auto& vertex = oVerts.emplace_back();
				vertex.position = algorithm::getElement(iPositions, svert[0]);
				vertex.textureCoordinates = algorithm::getElement(iTCoords, svert[1]);
				vertex.lightNormal = algorithm::getElement(iNormals, svert[2]);
			}
		}

		if (noNormal) { // take care of missing normals
			const Vector3 A{ oVerts[0].position - oVerts[1].position };
			const Vector3 B{ oVerts[2].position - oVerts[1].position };
			const Vector3 normal{ Vector3::cross(A, B) };

			for (auto& oVertex : oVerts) { oVertex.lightNormal = normal; }
		}
	}

	// Triangulate a list of vertices into a face by printing
	//	indicies corresponding with triangles within it
	void Loader::VertexTriangluation(std::vector<unsigned int>& oIndices,
		const std::vector<Vertex>& iVerts)
	{
		if (iVerts.size() < 3) { return; } // no triangle can be created
		else if (iVerts.size() == 3) { // if it is triangle no need to be calculated
			oIndices.push_back(0);
			oIndices.push_back(1);
			oIndices.push_back(2);
			return;
		}

		std::vector<Vertex> tVerts = iVerts; // Create a list of vertices

		while (true) {
			for (int i = 0; i < int(tVerts.size()); i++) { // For every vertex
				const auto pPrevious = [i, &tVerts]()->Vertex {
					if (i == 0) {
						return tVerts[tVerts.size() - 1]; 
					}
					
					return tVerts[i - 1];
				}();

				const auto pCurrent{ tVerts[i] };

				const auto pNext = [i, &tVerts]()->Vertex {
					if (i == (tVerts.size() - 1)) {
						return tVerts[0];
					}

					return tVerts[i + 1];
				}();

				// Check to see if there are only 3 verts left
				// if so this is the last triangle
				const auto tVertsSize{ (uint32_t)tVerts.size() };
				const auto iVertSize{ (uint32_t)iVerts.size() };
				if (tVertsSize == 3) {
					 
					for (uint32_t j = 0; j < tVertsSize; j++) { // Create a triangle from pCur, pPrev, pNext
						if (iVerts[j].position == pCurrent.position) { oIndices.push_back(j); }
						if (iVerts[j].position == pPrevious.position) { oIndices.push_back(j); }
						if (iVerts[j].position == pNext.position) { oIndices.push_back(j); }
					}

					tVerts.clear();
					break;
				}
				else if (tVertsSize == 4) {
					for (uint32_t j = 0; j < iVertSize; j++) { // Create a triangle from pCur, pPrev, pNext
						if (iVerts[j].position == pCurrent.position) { oIndices.push_back(j); }
						if (iVerts[j].position == pPrevious.position) { oIndices.push_back(j); }
						if (iVerts[j].position == pNext.position) { oIndices.push_back(j); }
					}

					const auto tempVec = [&tVerts, &pCurrent, &pNext, &pPrevious]()->Vector3 {
						const auto it = std::find_if(tVerts.cbegin(), tVerts.cend(), [&pCurrent, &pNext, &pPrevious](const auto& vertex) {
							return vertex.position != pCurrent.position && vertex.position != pPrevious.position && vertex.position != pNext.position;
						});

						if (it != tVerts.cend()) { return it->position; }

						return Vector3{};
					}();

					for (uint32_t j = 0; j < iVertSize; j++) {
						if (iVerts[j].position == tempVec) { oIndices.push_back(j); }
						if (iVerts[j].position == pPrevious.position) { oIndices.push_back(j); }
						if (iVerts[j].position == pNext.position) { oIndices.push_back(j); }
					}

					tVerts.clear();
					break;
				}

				// If Vertex is not an interior vertex
				const float angleRad{ Vector3::angleBetween(pPrevious.position - pCurrent.position, pNext.position - pCurrent.position) };
				const float angleDeg{ maths::trig::toDegrees(angleRad) };
				if (angleDeg <= 0.f && angleDeg >= 180.f) {continue; }
					
				// If any vertices are within this triangle
				const bool inTriangle = [&iVerts, &pCurrent, &pNext, &pPrevious]()->bool {
					const auto it = std::find_if(iVerts.cbegin(), iVerts.cend(), [&pCurrent, &pNext, &pPrevious](const auto& vertex) {

						const bool isInTriangle{ Vector3::inTriangle(vertex.position, pPrevious.position, pCurrent.position, pNext.position) };
						const bool notEqualToPrevious{ vertex.position != pPrevious.position };
						const bool notEqualToCurrent{ vertex.position != pCurrent.position };
						const bool notEqualToNext{ vertex.position != pNext.position };

						return isInTriangle && notEqualToPrevious && notEqualToCurrent && notEqualToNext;
					});

					if (it != iVerts.cend()) { return true; }
					return false;
				}();

				if (inTriangle) { continue; }

				for (uint32_t j = 0; j < iVertSize; j++) { // Create a triangle from pCur, pPrev, pNext
					if (iVerts[j].position == pCurrent.position) { oIndices.push_back(j); }
					if (iVerts[j].position == pPrevious.position) { oIndices.push_back(j); }
					if (iVerts[j].position == pNext.position) { oIndices.push_back(j); }
				}

				auto foundCurrentInList = [&pCurrent](const Vertex& vertex) {
					return vertex.position == pCurrent.position;
				};

				auto it = std::find_if(tVerts.begin(), tVerts.end(), foundCurrentInList);
				if (it != tVerts.end()) { tVerts.erase(it); }

				i = -1; // reset i to the start : -1 since loop will add 1 to it
			}

			if (oIndices.size() == 0) { break; } // if no triangles were created 
			if (tVerts.size() == 0) { break; } // if no more vertices
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