/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MeshCreator.h"


namespace mar {
	namespace graphics {


        std::vector<float> MeshCreator::Cube::getVertices() {
            return {
                //  front (x, y, z)		// LightNormal			// Texture		// ShapeIndex
                -1.0f, -1.0f,  1.0f,	-1.0f, -1.0f, 2.0f,		0.0f, 0.0f,		0.0f, // 0
                 1.0f, -1.0f,  1.0f,	2.0f, -2.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 1
                 1.0f,  1.0f,  1.0f,	1.0f, 1.0f, 2.0f,		1.0f, 1.0f,		0.0f, // 2
                -1.0f,  1.0f,  1.0f,	-2.0f, 2.0f, 1.0f,		0.0f, 1.0f,		0.0f, // 3
                //  back 							
                -1.0f, -1.0f, -1.0f,	-2.0f, -2.0f, -1.0f,		0.0f, 0.0f,		0.0f, // 4
                 1.0f, -1.0f, -1.0f,	1.0f, -1.0f, -2.0f,		1.0f, 0.0f,		0.0f, // 5
                 1.0f,  1.0f, -1.0f,	2.0f, 2.0f, -1.0f,		1.0f, 1.0f,		0.0f, // 6
                -1.0f,  1.0f, -1.0f,	-1.0f, 1.0f, -2.0f,		0.0f, 1.0f,		0.0f  // 7
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

        std::vector<float> MeshCreator::Wall::getVertices() {
            return {
                //  front (x, y, z)			// LightNormal			// Texture		// ShapeIndex
                -0.2f, -1.0f,  10.0f,		-1.f, -1.f, 3.f,		0.0f, 0.0f,		0.0f, // 0
                 0.2f, -1.0f,  10.0f,		2.f, -2.f, 2.f,		0.0f, 0.0f,		0.0f, // 1
                 0.2f,  5.0f,  10.0f,		1.f, 1.f, 3.f,		1.0f, 0.0f,		0.0f, // 2
                -0.2f,  5.0f,  10.0f,		-2.f, 2.f, 2.f,		1.0f, 0.0f,		0.0f, // 3
                //  back 								
                -0.2f, -1.0f, -10.0f,		-2.f, -2.f, 0.f,		0.0f, 1.0f,		0.0f, // 4
                 0.2f, -1.0f, -10.0f,		1.f, -1.f, -1.f,		0.0f, 1.0f,		0.0f, // 5
                 0.2f,  5.0f, -10.0f,		2.f, 2.f, 0.f,		1.0f, 1.0f,		0.0f, // 6
                -0.2f,  5.0f, -10.0f,		-1.f, 1.f, -1.f,		1.0f, 1.0f,		0.0f  // 7
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

        std::vector<float> MeshCreator::Surface::getVertices() {
            return {
                // (x, y, z)			// LightNormal			// TextureCoords	// ShapeIndex
                -15.0f, -1.0f,  15.0f,	0.f, 2.f, 1.f,		0.0f, 0.0f,			0.0f, // 0
                 15.0f, -1.0f,  15.0f,	0.f, 1.f, 1.f,		0.0f, 1.0f,			0.0f, // 1
                 15.0f, -1.0f, -15.0f,	0.f, 2.f, 1.f,		1.0f, 1.0f,			0.0f, // 2
                -15.0f, -1.0f, -15.0f,	0.f, 1.f, 1.f,		1.0f, 0.0f,			0.0f  // 3
            };
        }

        std::vector<uint32_t> MeshCreator::Surface::getIndices() {
            return {
                    0, 1, 2, // first triangle
                    2, 3, 0  // second triangle
            };
        }

        std::vector<float> MeshCreator::Pyramid::getVertices() {
            return {
                // (x, y, z)			// LightNormal			// TextureCoords // ShapeIndex
                -1.0f, -1.0f,  1.0f,	-0.894427f, 2.89443f, 1.89443f,		0.0f, 0.0f,		 0.0f,
                 1.0f, -1.0f,  1.0f,	0.894427f, 1.89443f, 1.89443f,		0.0f, 1.0f,		 0.0f,
                 1.0f, -1.0f, -1.0f,	0.894427f, 2.89443f, 0.105573f,		0.0f, 0.0f,		 0.0f,
                -1.0f, -1.0f, -1.0f,	-0.894427f, 1.89443f, 0.105573f,		0.0f, 1.0f,		 0.0f,
                 0.0f,  1.0f,  0.0f,	0.f, 1.78885f, 1.f,		0.5f, 0.5f,		 0.0f
            };
        }

        std::vector<uint32_t> MeshCreator::Pyramid::getIndices() {
            return {
                0, 1, 2,	2, 3, 0, // fundamental quad
                0, 1, 4,	1, 2, 4, // side triangles
                2, 3, 4,	3, 0, 4
            };
        }

		std::vector<float> MeshCreator::OBJ::vertices;
		std::vector<uint32_t> MeshCreator::OBJ::indices;

		void MeshCreator::OBJ::loadOBJ(const char* filename) {
			vertices.clear();
			indices.clear();

			std::vector<maths::vec3> method_vertices;
			std::vector<maths::vec2> method_vertices_texcoords;

			float input[3];

			std::ifstream stream(filename, std::ios::in);
			std::string line;

			if (!stream.is_open()) {
				GRAPHICS_ERROR("Cannot open object path!");
				return;
			}

			while (std::getline(stream, line)) {
				// VERTEX TEXTURE COORDINATES
				if (line.substr(0, 2) == "vt") {
					std::istringstream ss(line.substr(3));
					ss >> input[0] >> input[1];
					method_vertices_texcoords.push_back({ input[0] , input[1] });
				}
				// VERTICES
				else if (line[0] == 'v') {
					std::istringstream ss(line.substr(2));
					ss >> input[0] >> input[1] >> input[2];
					method_vertices.push_back({ input[0] , input[1] , input[2] });
				}
				// INDICES
				else if (line[0] == 'f') {
					std::istringstream ss(line.substr(2));

					while (!ss.eof()) {
						uint32_t indice;
						ss >> indice >> std::ws;
						indices.push_back(indice - 1);

						if (ss.peek() == '/') {
							ss.get();

							if (ss.peek() == '/') {
								ss.get();
								uint32_t normal_indice;
								ss >> normal_indice >> std::ws;
							}
							else {
								uint32_t tex_indice;
								ss >> tex_indice >> std::ws;

								if (ss.peek() == '/') {
									ss.get();
									uint32_t normal_indice;
									ss >> normal_indice >> std::ws;
								}
							}
						}
					}
				}
				else {}
			} 

			float light_normal{ 0.0f };
			float tex_coords[]{ 0.0f, 0.0f };

			for (uint32_t i = 0; i < method_vertices.size(); i++) {
				vertices.push_back(method_vertices[i].x);
				vertices.push_back(method_vertices[i].y);
				vertices.push_back(method_vertices[i].z);

				vertices.push_back(light_normal);
				vertices.push_back(light_normal);
				vertices.push_back(light_normal);
				
				if (i < method_vertices_texcoords.size()) {
					vertices.push_back(method_vertices_texcoords[i].x);
					vertices.push_back(method_vertices_texcoords[i].y);
				}
				else {
					vertices.push_back(tex_coords[0]);
					vertices.push_back(tex_coords[1]);
				}

				vertices.push_back(0.f);
			}

			ShapeManipulator::calculateNormals(vertices, indices, 3 + 3 + 2 + 1);

			GRAPHICS_TRACE("ObjectLoader has been set!");
		} 
		
		std::vector<float> MeshCreator::OBJ::getVertices() {
			return vertices;
		}

		std::vector<uint32_t> MeshCreator::OBJ::getIndices() {
			return indices;
		}



} }