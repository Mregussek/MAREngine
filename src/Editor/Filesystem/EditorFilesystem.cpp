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

						ss << "#ambientstrength " << light.ambientStrength.x << " " << light.ambientStrength.y << " " << light.ambientStrength.z << "\n";
						ss << "#diffusestrength " << light.diffuseStrength.x << " " << light.diffuseStrength.y << " " << light.diffuseStrength.z << "\n";
						ss << "#specularstrength " << light.specularStrength.x << " " << light.specularStrength.y << " " << light.specularStrength.z << "\n";
					
						ss << "#constant " << light.constant << "\n";
						ss << "#linear " << light.linear << "\n";
						ss << "#quadratic " << light.quadratic << "\n";
						ss << "#shininess " << light.shininess << '\n';
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
						ren.vertices = graphics::MeshCreator::getVertices_Cube();
						ren.indices = graphics::MeshCreator::getIndices_Cube();
					}
					else if (line.find("Surface") != std::string::npos) {
						ren.id = "Surface";
						ren.vertices = graphics::MeshCreator::getVertices_Surface();
						ren.indices = graphics::MeshCreator::getIndices_Surface();
					}
					else if (line.find("Wall") != std::string::npos) {
						ren.id = "Wall";
						ren.vertices = graphics::MeshCreator::getVertices_Wall();
						ren.indices = graphics::MeshCreator::getIndices_Wall();
					}
					else if (line.find("Pyramid") != std::string::npos) {
						ren.id = "Pyramid";
						ren.vertices = graphics::MeshCreator::getVertices_Pyramid();
						ren.indices = graphics::MeshCreator::getIndices_Pyramid();
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

					// #ambientstrength - 16
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(16));
					is >> arr[0] >> arr[1] >> arr[2];

					light.ambientStrength.x = arr[0];
					light.ambientStrength.y = arr[1];
					light.ambientStrength.z = arr[2];

					// #diffusestrength - 16
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(16));
					is >> arr[0] >> arr[1] >> arr[2];

					light.diffuseStrength.x = arr[0];
					light.diffuseStrength.y = arr[1];
					light.diffuseStrength.z = arr[2];

					// #specularstrength - 17
					std::getline(file, line);
					is.clear();
					is = std::istringstream(line.substr(17));
					is >> arr[0] >> arr[1] >> arr[2];

					light.specularStrength.x = arr[0];
					light.specularStrength.y = arr[1];
					light.specularStrength.z = arr[2];

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

		/*
		void ObjectLoader::loadObject(const char* path) {
			std::vector<maths::vec3> vertex_positions;
			std::vector<maths::vec3> vertex_normals;
			std::vector<maths::vec2> vertex_texcoords;
			std::vector<uint32_t> indices;
			std::vector<uint32_t> normal_indices;
			std::vector<uint32_t> texcoord_indices;

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
						uint32_t indice;
						ss >> indice >> std::ws;
						indices.push_back(indice - 1);

						if (ss.peek() == '/') {
							ss.get();

							if (ss.peek() == '/') {
								ss.get();
								uint32_t normal_indice;
								ss >> normal_indice >> std::ws;
								normal_indices.push_back(normal_indice - 1);
							}
							else {
								uint32_t tex_indice;
								ss >> tex_indice >> std::ws;
								texcoord_indices.push_back(tex_indice - 1);

								if (ss.peek() == '/') {
									ss.get();
									uint32_t normal_indice;
									ss >> normal_indice >> std::ws;
									normal_indices.push_back(normal_indice - 1);
								}
							}
						}
					}
				}
				else if (line[0] == 'o') {}
				else if (line[0] == '#') {}
				else if (line[0] == 's') {}
				else if (line[0] == 'g') {}
				else {} // end if statement
			} // end while loop

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> dist(0, 1);
			float red = (float)dist(mt);
			float green = (float)dist(mt);
			float blue = (float)dist(mt);

			s_defaultcolor = { red, green, blue };
			float light_normal[]{ 0.0f, 0.0f, 1.0f };
			float tex_coords[]{ 0.0f, 0.0f };

			s_indices = indices;
			s_name = path;
			s_layout = { 3, 3, 2, 1, 1 };
			s_texid = 0.f;
			s_id = 0.f;
			s_scale = { 1.f, 1.f, 1.f };
			s_center = { 0.0f, 0.0f, 0.0f };
			s_angle = { 0.f, 0.f, 0.f };

			if (!vertex_normals.empty())
				for (uint32_t i = 0; i < indices.size(); i += 3) {
					uint32_t a = indices[i];
					uint32_t b = indices[i + 1];
					uint32_t c = indices[i + 2];

					if (a >= vertex_positions.size() || b >= vertex_positions.size() ||
						c >= vertex_positions.size()) break;

					if (a >= vertex_normals.size() || b >= vertex_normals.size() ||
						c >= vertex_normals.size()) break;

					maths::vec3 U = vertex_positions[b] - vertex_positions[a];
					maths::vec3 V = vertex_positions[c] - vertex_positions[a];

					maths::vec3 normal = maths::vec3::normalize(maths::vec3::cross(U, V));

					vertex_normals[a] = normal;
					vertex_normals[b] = normal;
					vertex_normals[c] = normal;
				}

			s_vertices.clear();

			for (uint32_t i = 0; i < vertex_positions.size(); i++) {
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
		*/


} }