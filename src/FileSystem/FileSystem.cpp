/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "FileSystem.h"

namespace mar {
	namespace filesystem {

		Storage Storage::s_instance;

		void fnc::saveSceneToFile(const char* path, const std::vector<graphics::Mesh*>& meshes) {
			const char* c = "Going to save mesh to: ";
			MAR_CORE_INFO(c);
			MAR_CORE_INFO(path);
			
			std::ofstream ss(path, std::ios::out | std::ios::trunc);

			if (!ss.is_open()) {
				MAR_CORE_ERROR("Cannot open file and save scene!");
				return;
			}

			ss << "MAR ENGINE SCENE FILE" << std::endl;
			ss << "#BEGIN" << std::endl << std::endl;

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

				ss << "#mesh_id " + std::to_string(i) << std::endl;
				ss << "#mesh_type " + mesh_type << std::endl << std::endl;

				for (unsigned int j = 0; j < meshes[i]->getShapesCount(); j++) {
					auto shape = meshes[i]->getShape(j);
					auto center = shape->getCenter();
					auto angle = shape->getAngle();
					auto color = shape->getDefaultColor();

					ss << "#shape_id " + std::to_string(j) << std::endl;
					ss << "#shape_name " << shape->getName() << std::endl;
					ss << "#shape_center " + std::to_string(center.x) + " " + std::to_string(center.y)
						+ " " + std::to_string(center.z) << std::endl;
					ss << "#shape_angle " + std::to_string(angle.x) + " " + std::to_string(angle.y)
						+ " " + std::to_string(angle.z) << std::endl;
					ss << "#shape_color " + std::to_string(color.x) + " " + std::to_string(color.y)
						+ " " + std::to_string(color.z) << std::endl;
					ss << "#shape_texture " << shape->getUsedTexture() << std::endl;

					if (std::strcmp(shape->getName(), "Object") == 0)
						ss << "#shape_obj " << shape->getUsedObj() << std::endl;

					ss << std::endl;
				}
			}

			ss << "#END" << std::endl;

			ss.close();
			MAR_CORE_INFO("Saved!");
		}

		std::vector<layers::MeshLayer*> fnc::loadSceneFromFile(const char* path) {
			MAR_CORE_INFO("Going to read scene from: ");
			MAR_CORE_INFO(path);

			std::ifstream file(path);
			std::string line;
			std::vector<layers::MeshLayer*> loaded_layers;

			std::vector<graphics::SceneType> scene_type;
			std::vector<graphics::MeshType> mesh_type;

			std::vector<std::vector<Ref<graphics::Shape>>> shapes;
			std::vector<std::vector<glm::vec3>> centers;
			std::vector<std::vector<glm::vec3>> angles;
			std::vector<std::vector<glm::vec3>> colors;
			std::vector<std::vector<const char*>> textures;

			float input[3];
			std::string sinput;
			int shape_count{ 0 };
			int mesh_count{ -1 };
			int scene_count{ -1 };

			if (!file.is_open()) {
				MAR_CORE_ERROR("Cannot open file!");
				return std::vector<layers::MeshLayer*>();
			}

			loaded_layers.clear();

			while (std::getline(file, line)) {
				if (line.find("#mesh_id") != std::string::npos) {

					loaded_layers.push_back(new layers::MeshLayer());
					shapes.push_back(std::vector<Ref<graphics::Shape>>());
					centers.push_back(std::vector<glm::vec3>());
					angles.push_back(std::vector<glm::vec3>());
					colors.push_back(std::vector<glm::vec3>());
					textures.push_back(std::vector<const char*>());

					mesh_count++;
					shape_count = 0;
				}
				else if (line.find("#mesh_type") != std::string::npos) {
					if (line.find("normal") != std::string::npos)
						mesh_type.push_back(NORMAL_MESH_TYPE);
					else if (line.find("cubemaps") != std::string::npos)
						mesh_type.push_back(CUBEMAPS_MESH_TYPE);
					else if (line.find("objects") != std::string::npos)
						mesh_type.push_back(OBJECTS_MESH_TYPE);
				}
				else if (line.find("#shape_id") != std::string::npos) {
					shape_count++;
				}
				else if (line.find("#shape_name") != std::string::npos) {
					if (line.find("Cube") != std::string::npos)
						shapes[mesh_count].push_back(graphics::MeshCreator::createCube());
					else if (line.find("Pyramid") != std::string::npos)
						shapes[mesh_count].push_back(graphics::MeshCreator::createPyramid());
					else if (line.find("Surface") != std::string::npos)
						shapes[mesh_count].push_back(graphics::MeshCreator::createSurface());
					else if (line.find("Wall") != std::string::npos)
						shapes[mesh_count].push_back(graphics::MeshCreator::createWall());
					else if (line.find("Object") != std::string::npos)
						shapes[mesh_count].push_back(graphics::MeshCreator::createEmptyShape());
				}
				else if (line.find("#shape_center") != std::string::npos) {
					std::istringstream ss(line.substr(14));
					ss >> input[0] >> input[1] >> input[2];
					centers[mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_angle") != std::string::npos) {
					std::istringstream ss(line.substr(13));
					ss >> input[0] >> input[1] >> input[2];
					angles[mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_color") != std::string::npos) {
					std::istringstream ss(line.substr(13));
					ss >> input[0] >> input[1] >> input[2];
					colors[mesh_count].push_back({ input[0], input[1], input[2] });
				}
				else if (line.find("#shape_texture") != std::string::npos) {
					std::istringstream ss(line.substr(15));
					ss >> sinput;
					textures[mesh_count].push_back(sinput.c_str());
				}
				else if (line.find("#shape_obj") != std::string::npos) {
					if (mesh_type[mesh_count] != OBJECTS_MESH_TYPE) {
						MAR_CORE_ERROR("Cannot load obj file to not OBJECT_MESH_TYPE!");
						continue;
					}
					else {

					}
				}
				else if (line.find("#scene_id") != std::string::npos) {
					scene_count++;
				}
				else if (line.find("#scene_type") != std::string::npos) {
					if (line.find("default") != std::string::npos)
						scene_type.push_back(DEFAULT_SCENE);
					else if (line.find("cubemaps") != std::string::npos)
						scene_type.push_back(CUBEMAPS_SCENE);
					else if (line.find("objects") != std::string::npos)
						scene_type.push_back(OBJECTS_SCENE);
				}
				else if (line.find("#scene_mesh") != std::string::npos) {
					if (line.find("normal") != std::string::npos)
						mesh_type.push_back(NORMAL_MESH_TYPE);
					else if (line.find("cubemaps") != std::string::npos)
						mesh_type.push_back(CUBEMAPS_MESH_TYPE);
					else if (line.find("objects") != std::string::npos)
						mesh_type.push_back(OBJECTS_MESH_TYPE);
				}
			}

			file.close();
			MAR_CORE_TRACE("Scene has been loaded!");

			if (mesh_count != -1) {

			}

			if (scene_count != -1) {
				if (mesh_type.size() != scene_type.size()) 
					MAR_CORE_ERROR("Loaded types of mesh are not equal to the size of scene types size!");
				else {
					for (unsigned int i = 0; i < mesh_type.size(); i++) {
						std::string name = "Mesh Layer " + std::to_string(i);
						layers::MeshLayer* layer = new layers::MeshLayer(name.c_str());
						layer->initializeLayer(new graphics::Renderer(), new graphics::Mesh());
						layer->create(Storage::getInstance()->factory, Storage::getInstance()->usegui);
						layer->scene(scene_type[i], mesh_type[i]);
						loaded_layers.push_back(layer);
					}

					MAR_CORE_INFO("Created Mesh Layers!");
				}
			}

			return loaded_layers;
		}


} }
