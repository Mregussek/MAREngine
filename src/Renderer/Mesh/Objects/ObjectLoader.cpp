/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ObjectLoader.h"
#include "../../../Debug/Log.h"

namespace mar {
	namespace graphics { namespace objects {


		const char* ObjectLoader::s_name;
		std::vector<float> ObjectLoader::s_vertices;
		std::vector<unsigned int> ObjectLoader::s_indices;
		std::vector<unsigned int> ObjectLoader::s_layout;
		glm::vec3 ObjectLoader::s_center;
		glm::vec3 ObjectLoader::s_angle;
		glm::vec3 ObjectLoader::s_scale;
		glm::vec3 ObjectLoader::s_defaultcolor;
		float ObjectLoader::s_id;
		float ObjectLoader::s_texid;


		void ObjectLoader::loadObject(const char* path) {
			std::vector<glm::vec3> vertex_positions;
			std::vector<glm::vec3> vertex_normals;
			std::vector<glm::vec2> vertex_texcoords;
			std::vector<unsigned int> indices;
			std::vector<unsigned int> normal_indices;
			std::vector<unsigned int> texcoord_indices;

			float input[3];

			std::ifstream stream(path, std::ios::in);
			std::string line;

			if (!stream.is_open()) {
				MAR_CORE_ERROR("Cannot open object path!");
				return;
			}

			while (std::getline(stream, line)) {
				// VERTEX TEXTURE COORDINATES
				if (line.substr(0, 2) == "vt") {
					std::istringstream ss(line.substr(3));
					ss >> input[0] >> input[1];
					vertex_texcoords.push_back({ input[0] , input[1] });
				}
				// VERTEX NORMALS
				else if (line.substr(0, 2) == "vn") {
					std::istringstream ss(line.substr(3));
					ss >> input[0] >> input[1] >> input[2];
					vertex_normals.push_back({ input[0] , input[1] , input[2] });
				}
				// VERTICES
				else if (line[0] == 'v') {
					std::istringstream ss(line.substr(2));
					ss >> input[0] >> input[1] >> input[2];
					vertex_positions.push_back({ input[0] , input[1] , input[2] });
				}
				// INDICES
				else if (line[0] == 'f') {
					std::istringstream ss(line.substr(2));
					
					while (!ss.eof()) {
						unsigned int indice;
						ss >> indice >> std::ws;
						indices.push_back(indice - 1);

						if (ss.peek() == '/') {
							ss.get();

							if (ss.peek() == '/') {
								ss.get();
								unsigned int normal_indice;
								ss >> normal_indice >> std::ws;
								normal_indices.push_back(normal_indice - 1);
							}
							else {
								unsigned int tex_indice;
								ss >> tex_indice >> std::ws;
								texcoord_indices.push_back(tex_indice - 1);

								if (ss.peek() == '/') {
									ss.get();
									unsigned int normal_indice;
									ss >> normal_indice >> std::ws;
									normal_indices.push_back(normal_indice - 1);
								}
							}
						}
					}
				}
				else if (line[0] == 'o') { }
				else if (line[0] == '#') { }
				else if (line[0] == 's') { }
				else if (line[0] == 'g') { }
				else { } // end if statement
			} // end while loop

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> dist(0, 1);
			float red   = (float) dist(mt);
			float green = (float) dist(mt);
			float blue  = (float) dist(mt);

			s_defaultcolor = { red, green, blue };
			float light_normal[]{ 0.0f, 0.0f, 1.0f };
			float tex_coords[]{ 0.0f, 0.0f };

			s_indices = indices;
			s_name = "Object";
			s_layout = { 3, 3, 2, 1, 1 };
			s_texid = 0.f;
			s_id = 0.f;
			s_scale = { 1.f, 1.f, 1.f };
			s_center = { 0.0f, 0.0f, 0.0f };
			s_angle = { 0.f, 0.f, 0.f };

			if(!vertex_normals.empty())
				for (unsigned int i = 0; i < indices.size(); i += 3) {
					unsigned int a = indices[i];
					unsigned int b = indices[i + 1];
					unsigned int c = indices[i + 2];

					if (a >= vertex_positions.size() || b >= vertex_positions.size() ||
						c >= vertex_positions.size()) break;

					if (a >= vertex_normals.size() || b >= vertex_normals.size() ||
						c >= vertex_normals.size()) break;

					glm::vec3 U = vertex_positions[b] - vertex_positions[a];
					glm::vec3 V = vertex_positions[c] - vertex_positions[a];

					glm::vec3 normal = glm::normalize(glm::cross(U, V));

					vertex_normals[a] = normal;
					vertex_normals[b] = normal;
					vertex_normals[c] = normal;
				}

			s_vertices.clear();

			for (unsigned int i = 0; i < vertex_positions.size(); i++) {
				s_vertices.push_back(vertex_positions[i].x);
				s_vertices.push_back(vertex_positions[i].y);
				s_vertices.push_back(vertex_positions[i].z);

				if (i < vertex_normals.size()) {
					s_vertices.push_back(vertex_normals[i].x);
					s_vertices.push_back(vertex_normals[i].y);
					s_vertices.push_back(vertex_normals[i].z);
				}
				else {
					s_vertices.push_back(light_normal[0]);
					s_vertices.push_back(light_normal[1]);
					s_vertices.push_back(light_normal[2]);
				}
				
				if (i < vertex_texcoords.size()) {
					s_vertices.push_back(vertex_texcoords[i].x);
					s_vertices.push_back(vertex_texcoords[i].y);
				}
				else {
					s_vertices.push_back(tex_coords[0]);
					s_vertices.push_back(tex_coords[1]);
				}
				
				s_vertices.push_back(s_texid);
				s_vertices.push_back(s_id);
			}

			MAR_CORE_TRACE("ObjectLoader has been set!");
		} // end ObjectLoader::loadObject


} } }