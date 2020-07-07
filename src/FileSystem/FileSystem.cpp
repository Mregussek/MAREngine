/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "FileSystem.h"

namespace mar {
	namespace filesystem {


		std::vector<std::string> fnc::s_marfiles;
		std::vector<std::string> fnc::s_texturefiles;
		int fnc::s_mesh_count{ -1 };
		int fnc::s_scene_count{ -1 };
		std::vector<graphics::SceneType> fnc::s_scene_type;
		std::vector<graphics::MeshType> fnc::s_mesh_type;
		std::vector<std::vector<Ref<graphics::Shape>>> fnc::s_shapes;
		std::vector<std::vector<glm::vec3>> fnc::s_centers;
		std::vector<std::vector<glm::vec3>> fnc::s_angles;
		std::vector<std::vector<glm::vec3>> fnc::s_scales;
		std::vector<std::vector<glm::vec3>> fnc::s_colors;
		std::vector<std::vector<std::string>> fnc::s_textures;
		std::vector<std::vector<std::string>> fnc::s_objs;



		void fnc::updateMarFiles(const char* path) {
			s_marfiles.clear();

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				std::string s = entry.path().filename().string();
				s_marfiles.push_back(s);
			}
		}

		void fnc::updateMarTextures(const char* path) {
			s_texturefiles.clear();

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				std::string s = entry.path().filename().string();
				s_texturefiles.push_back(s);
			}
		}

		void fnc::saveSceneToFile(const char* path, const std::vector<graphics::Mesh*>& meshes) {
			const char* c = "Going to save mesh to: ";
			MAR_CORE_INFO(c);
			MAR_CORE_INFO(path);
			
			std::ofstream ss(path, std::ios::out | std::ios::trunc);

			if (!ss.is_open()) {
				MAR_CORE_ERROR("Cannot open file and save scene!");
				return;
			}

			ss << "MAR ENGINE SCENE FILE" << "\n";
			ss << "#BEGIN" << "\n" << "\n";

			////////////////////////////////////////////////
			/// --------- WRITING LOOP TO FILE --------- ///
			for (unsigned int i = 0; i < meshes.size(); i++) {
				std::string mesh_type;

				switch (meshes[i]->getMeshType()) {
				case graphics::MeshType::NORMAL:
					mesh_type = "normal";
					break;
				case graphics::MeshType::CUBEMAPS:
					mesh_type = "cubemaps";
					break;
				case graphics::MeshType::OBJECTS:
					mesh_type = "objects";
					break;
				}

				ss << "#mesh_id " + std::to_string(i) << "\n";
				ss << "#mesh_type " + mesh_type << "\n\n";

				for (unsigned int j = 0; j < meshes[i]->getShapesCount(); j++) {
					auto shape = meshes[i]->getShape(j);
					auto center = shape->getCenter();
					auto angle = shape->getAngle();
					auto scale = shape->getScale();
					auto color = shape->getDefaultColor();

					ss << "#shape_id " + std::to_string(j) << "\n";
					ss << "#shape_name " << shape->getName() << "\n";
					ss << "#shape_center " + std::to_string(center.x) + " " + std::to_string(center.y)
						+ " " + std::to_string(center.z) << "\n";
					ss << "#shape_angle " + std::to_string(angle.x) + " " + std::to_string(angle.y)
						+ " " + std::to_string(angle.z) << "\n";
					ss << "#shape_scale " + std::to_string(scale.x) + " " + std::to_string(scale.y)
						+ " " + std::to_string(scale.z) << "\n";
					ss << "#shape_color " + std::to_string(color.x) + " " + std::to_string(color.y)
						+ " " + std::to_string(color.z) << "\n";
					ss << "#shape_texture " << shape->getUsedTexture() << "\n";

					if (std::strcmp(shape->getName(), "Object") == 0)
						ss << "#shape_obj " << shape->getUsedObj() << "\n";

					ss << "\n";
				}
			}
			/// ------- END WRITING LOOP TO FILE ------- ///
			////////////////////////////////////////////////

			ss << "#END" << "\n";

			ss.close();
			MAR_CORE_INFO("Saved!");
		}

		void fnc::loadSceneFromFile(std::string path) {
			MAR_CORE_INFO("Going to read scene from: ");
			MAR_CORE_INFO(path);

			std::ifstream file(path);

			if (!file.is_open()) {
				MAR_CORE_ERROR("Cannot open file!");
				return;
			}

			std::string line;
			float input[3];
			
			std::vector<layers::MeshLayer*> loaded_layers;
			s_mesh_count = -1;
			s_scene_count = -1;
			s_scene_type.clear();
			s_mesh_type.clear();
			s_shapes.clear();
			s_centers.clear();
			s_angles.clear();
			s_scales.clear();
			s_colors.clear();
			s_textures.clear();
			s_objs.clear();

			//////////////////////////////////////////////////
			/// --------- READING LOOP FROM FILE --------- ///
			while (std::getline(file, line)) {
				if (line.find("#mesh_id") != std::string::npos) {

					s_shapes.push_back(std::vector<Ref<graphics::Shape>>());
					s_centers.push_back(std::vector<glm::vec3>());
					s_angles.push_back(std::vector<glm::vec3>());
					s_scales.push_back(std::vector<glm::vec3>());
					s_colors.push_back(std::vector<glm::vec3>());
					s_textures.push_back(std::vector<std::string>());
					s_objs.push_back(std::vector<std::string>());

					s_mesh_count++;
				}
				else if (line.find("#mesh_type") != std::string::npos) {
					if (line.find("normal") != std::string::npos)
						s_mesh_type.push_back(NORMAL_MESH_TYPE);
					else if (line.find("cubemaps") != std::string::npos)
						s_mesh_type.push_back(CUBEMAPS_MESH_TYPE);
					else if (line.find("objects") != std::string::npos)
						s_mesh_type.push_back(OBJECTS_MESH_TYPE);
				}
				else if (line.find("#shape_id") != std::string::npos) {

				}
				else if (line.find("#shape_name") != std::string::npos) {
					if (line.find("Cube") != std::string::npos)
						s_shapes[s_mesh_count].push_back(graphics::MeshCreator::createCube());
					else if (line.find("Pyramid") != std::string::npos)
						s_shapes[s_mesh_count].push_back(graphics::MeshCreator::createPyramid());
					else if (line.find("Surface") != std::string::npos)
						s_shapes[s_mesh_count].push_back(graphics::MeshCreator::createSurface());
					else if (line.find("Wall") != std::string::npos)
						s_shapes[s_mesh_count].push_back(graphics::MeshCreator::createWall());
					else if (line.find("Object") != std::string::npos)
						s_shapes[s_mesh_count].push_back(graphics::MeshCreator::createEmptyShape());
				}
				else if (line.find("#shape_center") != std::string::npos) {
					std::istringstream ss(line.substr(14));
					ss >> input[0] >> input[1] >> input[2];
					s_centers[s_mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_angle") != std::string::npos) {
					std::istringstream ss(line.substr(13));
					ss >> input[0] >> input[1] >> input[2];
					s_angles[s_mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_scale") != std::string::npos) {
					std::istringstream ss(line.substr(13));
					ss >> input[0] >> input[1] >> input[2];
					s_scales[s_mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_color") != std::string::npos) {
					std::istringstream ss(line.substr(13));
					ss >> input[0] >> input[1] >> input[2];
					s_colors[s_mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_texture") != std::string::npos) {
					std::istringstream ss(line.substr(15));
					std::string sinput;
					ss >> sinput;
					s_textures[s_mesh_count].push_back(sinput);
				}
				else if (line.find("#shape_obj") != std::string::npos) {
					if (s_mesh_type[s_mesh_count] != OBJECTS_MESH_TYPE) {
						MAR_CORE_ERROR("Cannot load obj file to not OBJECT_MESH_TYPE!");
						continue;
					}
					else {
						std::istringstream ss(line.substr(11));
						std::string sinput;
						ss >> sinput;
						s_objs[s_mesh_count].push_back(sinput);
					}
				}
				/* DEFAULT SCENE LOADERS (from engine code) */
				else if (line.find("#scene_id") != std::string::npos) {
					s_scene_count++;
				}
				else if (line.find("#scene_type") != std::string::npos) {
					if (line.find("default") != std::string::npos)
						s_scene_type.push_back(DEFAULT_SCENE);
					else if (line.find("cubemaps") != std::string::npos)
						s_scene_type.push_back(CUBEMAPS_SCENE);
					else if (line.find("objects") != std::string::npos)
						s_scene_type.push_back(OBJECTS_SCENE);
				}
				else if (line.find("#scene_mesh") != std::string::npos) {
					if (line.find("normal") != std::string::npos)
						s_mesh_type.push_back(NORMAL_MESH_TYPE);
					else if (line.find("cubemaps") != std::string::npos)
						s_mesh_type.push_back(CUBEMAPS_MESH_TYPE);
					else if (line.find("objects") != std::string::npos)
						s_mesh_type.push_back(OBJECTS_MESH_TYPE);
				}
			}
			/// ------- END READING LOOP FROM FILE ------- ///
			//////////////////////////////////////////////////

			file.close();
			MAR_CORE_TRACE("Scene has been loaded!");
		}


} }
