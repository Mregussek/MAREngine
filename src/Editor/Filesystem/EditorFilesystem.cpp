/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EditorFilesystem.h"
#include "../../Core/graphics/Mesh/MeshCreator.h"
#include "../../Debug/Log.h"


namespace mar {
	namespace editor {


		void Filesystem::saveToFile(ecs::Scene* scene, const char* filename) {
			MAR_CORE_INFO("FILESYSTEM: going to save scene at:");
			MAR_CORE_INFO(filename);

			std::ofstream ss(filename, std::ios::out | std::ios::trunc);

			if (!ss.is_open()) {
				MAR_CORE_ERROR("Cannot open file and save scene!");
				return;
			}

			std::string name = scene->getName();
			if (name == "EmptyScene")
				name = std::string(filename);

			ss << "MAREngine scene file\n\n";

			ss << "#Scene\n";
			ss << "#Scene_Name " << name << "\n";

			for (auto& entity : scene->entities) {
				ss << "\n#Entity\n";

				auto& com = entity.getComponent<ecs::Components>();

				for(auto& component : com.components) {

					if (component == ECS_TAG) {
						auto& tag = entity.getComponent<ecs::TagComponent>();
						ss << "#TagComponent " << tag.tag << "\n";
					}
					else if (component == ECS_RENDERABLE) {
						auto& renderable = entity.getComponent<ecs::RenderableComponent>();
						ss << "#RenderableComponent " << renderable.id << "\n";
					}
					else if (component == ECS_TRANSFORM) {
						auto& transform = entity.getComponent<ecs::TransformComponent>();
						ss << "#TransformComponent Begin\n";
						ss << "#center " << transform.center.x << " " << transform.center.y << " " << transform.center.z << "\n";
						ss << "#angles " << transform.angles.x << " " << transform.angles.y << " " << transform.angles.z << "\n";
						ss << "#scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z << "\n";
						ss << "#general_scale " << transform.general_scale << "\n";
					}
					else if (component == ECS_COLOR) {
						auto& color = entity.getComponent<ecs::ColorComponent>();
						ss << "#ColorComponent " << color.color.x << " " << color.color.y << " " << color.color.z << "\n";
					}
					else if (component == ECS_TEXTURE2D) {
						auto& tex = entity.getComponent<ecs::Texture2DComponent>();
						ss << "#Texture2DComponent " << tex.texture << "\n";
					}
					else if (component == ECS_CUBEMAP) {
						auto& cube = entity.getComponent<ecs::TextureCubemapComponent>();
						ss << "#TextureCubemapComponent " << cube.cubemap << "\n";
					}
					else if (component == ECS_LIGHT) {
						auto& light = entity.getComponent<ecs::LightComponent>();

						ss << "#LightComponent Begin\n";

						ss << "#ambientlight " << light.ambient.x << " " << light.ambient.y << " " << light.ambient.z << "\n";
						ss << "#diffuselight " << light.diffuse.x << " " << light.diffuse.y << " " << light.diffuse.z << "\n";
						ss << "#specularlight " << light.specular.x << " " << light.specular.y << " " << light.specular.z << "\n";

						ss << "#constant " << light.constant << "\n";
						ss << "#linear " << light.linear << "\n";
						ss << "#quadratic " << light.quadratic << "\n";
						ss << "#shininess " << light.shininess << '\n';
					}
					else if (component == ECS_CAMERA) {
						auto& cam = entity.getComponent<ecs::CameraComponent>();

						ss << "#CameraComponent Begin\n";
					}

 				}
			}

			MAR_CORE_INFO("FILESYSTEM: scene has been saved!");
		}

		ecs::Scene* Filesystem::openFile(const char* filename) {
			MAR_CORE_INFO("FILESYSTEM: going to load scene from:");
			MAR_CORE_INFO(filename);

			ecs::Scene* scene = nullptr;

			std::ifstream file(filename);
			if (!file.is_open()) {
				MAR_CORE_ERROR("Cannot open file!");
				return nullptr;
			}

			std::string line;
			ecs::Entity* currentEntity{ nullptr };

			while (std::getline(file, line)) {
				if (line.find("#Scene_Name") != std::string::npos) {
					scene = new ecs::Scene("empty");

					std::istringstream is(line.substr(12));
					std::string new_scene_name;
					is >> new_scene_name;
					scene->setName(new_scene_name);
				}
				else if (line.find("#Entity") != std::string::npos) {
					currentEntity = &scene->createEntity();
				}
				else if (line.find("#TagComponent") != std::string::npos) {
					std::istringstream is(line.substr(14));
					std::string new_tag;
					is >> new_tag;
					auto& tag = currentEntity->getComponent<ecs::TagComponent>();
					tag.tag = new_tag;
				}
				else if (line.find("#TransformComponent") != std::string::npos) {
					auto& tran = currentEntity->addComponent<ecs::TransformComponent>(ECS_TRANSFORM);
					
					// #center - 7 letters
					std::getline(file, line);
					std::istringstream is(line.substr(7));
					float arr[3];
					is >> arr[0] >> arr[1] >> arr[2];

					tran.center.x = arr[0];
					tran.center.y = arr[1];
					tran.center.z = arr[2];

					// #angles - 6 letters
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(7));
					is >> arr[0] >> arr[1] >> arr[2];

					tran.angles.x = arr[0];
					tran.angles.y = arr[1];
					tran.angles.z = arr[2];

					// #scale - 6  letters
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(6));
					is >> arr[0] >> arr[1] >> arr[2];

					tran.scale.x = arr[0];
					tran.scale.y = arr[1];
					tran.scale.z = arr[2];

					// #general_scale - 12 letters
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(12));
					is >> arr[0];

					tran.general_scale = arr[0];

					ecs::System::handleTransformComponent(tran);
				}
				else if (line.find("#RenderableComponent") != std::string::npos) {
					auto& ren = currentEntity->addComponent<ecs::RenderableComponent>(ECS_RENDERABLE);

					if (line.find("Cube") != std::string::npos) {
						ren.id = "Cube";
						ren.vertices = graphics::MeshCreator::Cube::getVertices();
						ren.indices =  graphics::MeshCreator::Cube::getIndices();
					}
					else if (line.find("Surface") != std::string::npos) {
						ren.id = "Surface";
						ren.vertices = graphics::MeshCreator::Surface::getVertices();
						ren.indices =  graphics::MeshCreator::Surface::getIndices();
					}
					else if (line.find("Wall") != std::string::npos) {
						ren.id = "Wall";
						ren.vertices = graphics::MeshCreator::Wall::getVertices();
						ren.indices =  graphics::MeshCreator::Wall::getIndices();
					}
					else if (line.find("Pyramid") != std::string::npos) {
						ren.id = "Pyramid";
						ren.vertices = graphics::MeshCreator::Pyramid::getVertices();
						ren.indices =  graphics::MeshCreator::Pyramid::getIndices();
					}
					else if (line.find(".obj") != std::string::npos) {
						std::istringstream iss(line.substr(21));
						std::string name;
						iss >> name;
						ren.id = name;
						graphics::MeshCreator::OBJ::loadOBJ(name.c_str());
						ren.vertices = graphics::MeshCreator::OBJ::vertices;
						ren.indices = graphics::MeshCreator::OBJ::indices;
					}
				}
				else if (line.find("#ColorComponent") != std::string::npos) {
					std::istringstream is(line.substr(16));
					float arr[3];
					is >> arr[0] >> arr[1] >> arr[2];

					auto& color = currentEntity->addComponent<ecs::ColorComponent>(ECS_COLOR);
					color.color.x = arr[0];
					color.color.y = arr[1];
					color.color.z = arr[2];
				}
				else if (line.find("#LightComponent") != std::string::npos) {
					auto& light = currentEntity->addComponent<ecs::LightComponent>(ECS_LIGHT);

					// #ambientlight - 13
					std::getline(file, line);
					std::istringstream is(line.substr(13));
					float arr[3];
					is >> arr[0] >> arr[1] >> arr[2];

					light.ambient.x = arr[0];
					light.ambient.y = arr[1];
					light.ambient.z = arr[2];

					// #diffuselight - 13
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(13));
					is >> arr[0] >> arr[1] >> arr[2];

					light.diffuse.x = arr[0];
					light.diffuse.y = arr[1];
					light.diffuse.z = arr[2];

					// #specularlight - 14
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(14));
					is >> arr[0] >> arr[1] >> arr[2];

					light.specular.x = arr[0];
					light.specular.y = arr[1];
					light.specular.z = arr[2];

					// #constant - 9
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(9));
					is >> arr[0];

					light.constant = arr[0];
				
					// #linear - 7
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(7));
					is >> arr[0];

					light.linear = arr[0];

					// #quadratic - 10
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(10));
					is >> arr[0];

					light.quadratic = arr[0];

					// #shininess - 10
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(10));
					is >> arr[0];

					light.shininess = arr[0];

				}
			}

			file.close();

			MAR_CORE_INFO("FILESYSTEM: returning loaded scene");

			return scene;
		}


} }