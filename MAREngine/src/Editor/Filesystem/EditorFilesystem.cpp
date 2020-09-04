/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EditorFilesystem.h"
#include "../../Core/ecs/ECS/Systems.h"
#include "../../Core/graphics/Mesh/MeshCreator.h"
#include "../EditorLogging.h"
#include "Filesystem_SaveFile.h"


namespace mar {
	namespace editor {


		void Filesystem::saveToFile(ecs::Scene* scene, const char* filename) {
			EDITOR_INFO("FILESYSTEM: going to save scene {} at: {}", scene->getName(), filename);

			std::ofstream ss(filename, std::ios::out | std::ios::trunc);

			if (!ss.is_open()) {
				EDITOR_ERROR("Cannot open file {} and save scene at {}!", scene->getName(), filename);
				return;
			}

			std::string name = scene->getName();
			if (name == "EmptyScene")
				name = std::string(filename);

			ss << "MAREngine scene file\n\n";

			auto& back = scene->getBackground();

			ss << "#Scene\n";
			ss << "#Scene_Name " << name << "\n";
			ss << "#Scene_Background " << back.x << " " << back.y << " " << back.z << "\n";

			for (auto& entity : scene->getEntities()) {
				Filesystem_SaveFile::saveEntity(ss, entity);
			}

			for (auto& collection : scene->getCollections()) {
				Filesystem_SaveFile::saveCollection(ss, collection);
			}

			MAR_CORE_INFO("FILESYSTEM: scene has been saved!");
		}

		ecs::Scene* Filesystem::openFile(std::string filename) {
			EDITOR_INFO("FILESYSTEM: going to load scene from: {}", filename);

			std::ifstream file(filename);
			if (!file.is_open()) {
				if (filename == "BrandNewScene") {
					return ecs::Scene::createEmptyScene("BrandNewScene");
				}
				else {
					EDITOR_ERROR("Cannot open file {}, returning empty scene!", filename);
					return ecs::Scene::createEmptyScene("EmptySceneNotLoaded");
				}
			}

			std::string line;
			ecs::Scene* scene{ nullptr };
			ecs::Entity* currentEntity{ nullptr };

			while (std::getline(file, line)) {
				if (line.find("#Scene_Name") != std::string::npos) {
					std::istringstream is(line.substr(12));
					std::string new_scene_name;
					is >> new_scene_name;
					scene = new ecs::Scene(new_scene_name);

					// #Scene_Background - 19
					float arr[3];
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(19));
					is >> arr[0] >> arr[1] >> arr[2];
					scene->setBackground({ arr[0], arr[1], arr[2] });
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
					auto& tran = currentEntity->getComponent<ecs::TransformComponent>();
					
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
					color.texture.x = arr[0];
					color.texture.y = arr[1];
					color.texture.z = arr[2];
				}
				else if (line.find("Texture2DComponent") != std::string::npos) {
					std::istringstream iss(line.substr(19));
					std::string tex;
					iss >> tex;

					auto& texture = currentEntity->addComponent<ecs::Texture2DComponent>(ECS_TEXTURE2D);
					texture.texture = tex;
				}
				else if (line.find("#TextureCubemapComponent") != std::string::npos) {
					std::istringstream iss(line.substr(25));
					std::string tex;
					iss >> tex;

					auto& cubemap = currentEntity->addComponent<ecs::TextureCubemapComponent>(ECS_CUBEMAP);
					cubemap.texture = tex;
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
				else if (line.find("#CameraComponent") != std::string::npos) {
					auto& cam = currentEntity->addComponent<ecs::CameraComponent>(ECS_CAMERA);
					std::string type;
					float var;

					// #id - 3
					std::getline(file, line);
					std::istringstream iss(line.substr(3));
					iss >> type;

					cam.id = type;

					// #used - 5
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(5));
					type.clear();
					iss >> type;

					if (type.find("perspective") != std::string::npos) cam.Perspective = true;
					else cam.Perspective = false;

					// #type - 5
					std::getline(file, line);

					// #fov
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(4));
					iss >> var;

					cam.p_fov = var;

					// #aspectratio
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(12));
					iss >> var;

					cam.p_aspectRatio = var;

					// #near
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(5));
					iss >> var;

					cam.p_near = var;

					// #far
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(4));
					iss >> var;

					cam.p_far = var;
					
					// #type - 5
					std::getline(file, line);

					// #left
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(5));
					iss >> var;

					cam.o_left = var;

					// #right
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(6));
					iss >> var;

					cam.o_right = var;

					// #top
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(4));
					iss >> var;

					cam.o_top = var;

					// #bottom
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(7));
					iss >> var;

					cam.o_bottom = var;

					// #near
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(5));
					iss >> var;

					cam.o_near = var;

					// #far
					std::getline(file, line);
					iss.clear();
					iss = std::istringstream(line.substr(4));
					iss >> var;

					cam.o_far = var;

					if (cam.id.find("main") != std::string::npos) {
						auto& scene_camera = scene->getRenderCamera();

						if (cam.Perspective) {
							scene_camera.projection = maths::mat4::perspective(
								maths::Trig::toRadians(cam.p_fov),
								cam.p_aspectRatio,
								cam.p_near,
								cam.p_far
							);
						}
						else {
							scene_camera.projection = maths::mat4::orthographic(
								cam.o_left,
								cam.o_right,
								cam.o_top,
								cam.o_bottom,
								cam.o_near,
								cam.o_far
							);
						}

						auto& tran = currentEntity->getComponent<ecs::TransformComponent>();

						scene_camera.view = maths::mat4::lookAt(
							tran.center,
							{ 0.f, 0.f, -1.f },
							{ 0.f, 1.0f, 0.f }
						);

						scene_camera.model = maths::mat4::translation({ 0.f, 0.f, 0.f });
					}
				}
				else if (line.find("#ScriptComponent") != std::string::npos) {
					auto& script = currentEntity->addComponent<ecs::ScriptComponent>(ECS_SCRIPT);
					std::istringstream iss(line.substr(17));
					std::string s;
					iss >> s;
					script.script = s;
				}
			}

			file.close();

			EDITOR_INFO("FILESYSTEM: returning loaded scene {} from file {}", scene->getName(), filename);

			return scene;
		}

		std::string Filesystem::loadPyScript(const char* filename) {
			EDITOR_INFO("FILESYSTEM: going to load python script from: {}", filename);

			std::ifstream file(filename);
			if (!file.is_open()) {
				EDITOR_ERROR("Cannot open script file {}, returning empty string!", filename);
				return "";
			}

			std::string rtn{ "" };
			std::string line;

			while (std::getline(file, line)) {
				rtn += line + "\n";
			}

			return rtn;
		}

		void Filesystem::savePyScript(const char* filename, std::string source) {
			EDITOR_INFO("FILESYSTEM: going to save PyScript at: {}", filename);

			std::ofstream ss(filename, std::ios::out | std::ios::trunc);

			if (!ss.is_open()) {
				EDITOR_ERROR("FILESYSTEM: Cannot save python script - {}!", filename);
				return;
			}

			ss << std::move(source);

			ss.close();

			EDITOR_INFO("FILESYSTEM: saved PyScript at: {}", filename);
		}


} }