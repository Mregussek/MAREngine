/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ObjectLoader.h"
#include "../../../Debug/Log.h"

namespace mar {
	namespace graphics { namespace objects {


		std::string ObjectLoader::s_name;
		std::vector<float> ObjectLoader::s_vertices;
		std::vector<unsigned int> ObjectLoader::s_indices;
		std::vector<unsigned int> ObjectLoader::s_layout;
		glm::vec3 ObjectLoader::s_center;
		glm::vec3 ObjectLoader::s_angle;
		unsigned int ObjectLoader::s_id;
		unsigned int  ObjectLoader::s_texid;


		void ObjectLoader::loadObject(const std::string& path) {
			std::vector<glm::vec3> vertex_positions;
			std::vector<unsigned int> indices;

			glm::vec3 tmp_vec3;
			std::string tmp_prefix;

			std::stringstream ss;
			std::ifstream stream(path);
			std::string line;

			if (!stream.is_open()) {
				MAR_CORE_ERROR("Cannot open object path!");
				return;
			}

			while (std::getline(stream, line)) {
				ss.clear();
				ss.str(line);
				ss >> tmp_prefix;

				if (line.find("v") != std::string::npos) {
					ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
					vertex_positions.push_back(tmp_vec3);
				}
				else if (line.find("f") != std::string::npos) {
					unsigned int tmp_number;
					unsigned int counter = 0;

					while (ss >> tmp_number) {
						switch (counter) {
						case 0:
							indices.push_back(tmp_number);
							break;
						default: break;
						}

						if (ss.peek() == '/') {
							counter++;
							ss.ignore(1, '/');
						}
						else if (ss.peek() == ' ') {
							counter = 0;
							ss.ignore(1, ' ');
						}
						if (counter > 2) counter = 0;
					}
				}
				else if (line.find("vt") != std::string::npos) { }
				else if (line.find("vn") != std::string::npos) { }
				else if (line.find("o") != std::string::npos) { }
				else if (line.find("#") != std::string::npos) { }
				else if (line.find("s") != std::string::npos) {	}
				else if (line.find("g") != std::string::npos) {	}
				else { } // end if statement
			} // end while loop

			glm::vec3 basic_color{ 0.5f, 0.5f, 0.5f };
			glm::vec3 light_normal{ 0.0f, 0.0f, 0.0f };
			glm::vec2 tex_coords{ 0.0f, 0.0f };

			s_indices = indices;
			s_name = "Loaded Object";
			s_layout = { 3, 3, 3, 2, 1, 1 };
			s_texid = 0.f;
			s_id = 0.f;
			s_center = { 0.0f, -1.0f, 2.0f };
			s_angle = { 90.0f, 180.0f, 90.0f };
			
			for (unsigned int i = 0; i < vertex_positions.size(); i++) {
				// x, y, z coords
				s_vertices.push_back(vertex_positions[i].x / 5.0f);
				s_vertices.push_back(vertex_positions[i].y / 5.0f);
				s_vertices.push_back(vertex_positions[i].z / 5.0f);

				// basic colors
				s_vertices.push_back(basic_color.x);
				s_vertices.push_back(basic_color.y);
				s_vertices.push_back(basic_color.z);

				// light normals
				s_vertices.push_back(light_normal.x);
				s_vertices.push_back(light_normal.y);
				s_vertices.push_back(light_normal.z);

				// texture coords
				s_vertices.push_back(tex_coords.x);
				s_vertices.push_back(tex_coords.y);

				// tex index
				s_vertices.push_back(s_texid);

				// shape index
				s_vertices.push_back(s_id);
			}

			MAR_CORE_TRACE("ObjectLoader has been set!");
		} // end ObjectLoader::loadObject


} } }