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
						ss << "#ColorComponent " << color.texture.x << " " << color.texture.y << " " << color.texture.z << "\n";
					}
					else if (component == ECS_TEXTURE2D) {
						auto& tex = entity.getComponent<ecs::Texture2DComponent>();
						ss << "#Texture2DComponent " << tex.texture << "\n";
					}
					else if (component == ECS_CUBEMAP) {
						auto& cube = entity.getComponent<ecs::TextureCubemapComponent>();
						ss << "#TextureCubemapComponent " << cube.texture << "\n";
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
						ss << "#id " << cam.id << "\n";

						if (cam.Perspective)
							ss << "#used perspective\n";
						else 
							ss << "#used orthographic\n";

						ss << "#type perspective\n";
						ss << "#fov " << cam.p_fov << "\n";
						ss << "#aspectratio " << cam.p_aspectRatio << "\n";
						ss << "#near " << cam.p_near << "\n";
						ss << "#far " << cam.p_far << "\n";

						ss << "#type orthographic\n";
						ss << "#left " << cam.o_left << "\n";
						ss << "#right " << cam.o_right << "\n";
						ss << "#top " << cam.o_top << "\n";
						ss << "#bottom " << cam.o_bottom << "\n";
						ss << "#near " << cam.o_far << "\n";
						ss << "#far " << cam.o_far << "\n";
					}
					else if (component == ECS_SCRIPT) {
						auto& script = entity.getComponent<ecs::ScriptComponent>();

						ss << "#ScriptComponent " << script.script << "\n";
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
				return new ecs::Scene("EmptyScene");
			}

			std::string line;
			ecs::Entity* currentEntity{ nullptr };

			while (std::getline(file, line)) {
				if (line.find("#Scene_Name") != std::string::npos) {
					std::istringstream is(line.substr(12));
					std::string new_scene_name;
					is >> new_scene_name;
					scene = new ecs::Scene(new_scene_name);
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
						if (cam.Perspective) {
							scene->scene_camera.projection = maths::mat4::perspective(
								maths::Trig::toRadians(cam.p_fov),
								cam.p_aspectRatio,
								cam.p_near,
								cam.p_far
							);
						}
						else {
							scene->scene_camera.projection = maths::mat4::orthographic(
								cam.o_left,
								cam.o_right,
								cam.o_top,
								cam.o_bottom,
								cam.o_near,
								cam.o_far
							);
						}

						auto& tran = currentEntity->getComponent<ecs::TransformComponent>();

						scene->scene_camera.view = maths::mat4::lookAt(
							tran.center,
							{ 0.f, 0.f, -1.f },
							{ 0.f, 1.0f, 0.f }
						);

						scene->scene_camera.model = maths::mat4::translation({ 0.f, 0.f, 0.f });

						scene->useEditorCamera = false;
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

			MAR_CORE_INFO("FILESYSTEM: returning loaded scene");

			return scene;
		}

		std::string Filesystem::loadPyScript(const char* filename) {
			MAR_CORE_INFO("FILESYSTEM: going to load python script from:");
			MAR_CORE_INFO(filename);

			std::ifstream file(filename);
			if (!file.is_open()) {
				MAR_CORE_ERROR("Cannot open script file!");
				return "empty";
			}

			std::string rtn{ "" };
			std::string line;

			while (std::getline(file, line)) {
				rtn += line + "\n";
			}

			return rtn;
		}


} }