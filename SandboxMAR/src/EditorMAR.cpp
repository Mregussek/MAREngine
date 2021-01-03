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


#include "EditorMAR.h"
//#include "MAREngine/src/Debug/Profiler.h"


namespace marengine {


	void EditorMAR::initialize() {
		m_engine.initialize();

		//projectSelectionWindow();
	}
	/*
	void EditorMAR::projectSelectionWindow() {
		window::Window<GLFWwindow> window{};
		window.initialize(1200, 700, "MAREngine - Project Manager - (C) 2020-present Mateusz Rzeczyca");

		editor::ProjectSelectionGUI gui;

		gui.initialize("#version 330");

		while (!window.isGoingToClose())
		{
			platforms::SetupOpenGL::clearScreen({0.f, 1.0f, 1.0f});

			gui.prepare();
			gui.update();
			gui.render();

			window.swapBuffers();
		}

		gui.shutdown();
		window.terminate();
	}
	*/
	
	Scene* oldFileLoading(const char* path);

	void EditorMAR::runProjectOnEngine() {
		WindowInstance<GLFWwindow> displayWindow{};
		LayerStack layerStack{};

		//auto* scene = FSceneDeserializer::loadSceneFromFile(m_engine.getPathToLoad());
		auto* scene = oldFileLoading("DefaultProject/Scenes/DefaultProject.marscene");
		//auto* scene = Scene::createEmptyScene("EmptyScene");

		RenderLayer renderLayer("Render Layer");
		layerStack.pushLayer(&renderLayer);

		SceneLayer sceneLayer("Scene Layer");
		sceneLayer.passSceneToManager(scene);
		layerStack.pushLayer(&sceneLayer);

		EditorLayer editorLayer("Editor Layer");
		layerStack.pushOverlay(&editorLayer);

		displayWindow.initialize(1600, 900, m_engine.getName());

		layerStack.initialize();

		while (!displayWindow.isGoingToClose() && !m_engine.shouldEngineRestart()) {

			layerStack.update();

		}

		layerStack.close();
		displayWindow.terminate();
	}

	void EditorMAR::shutdown() {
		while (m_engine.shouldEngineRestart()) {
			m_engine.setNoRestart();
			runProjectOnEngine();
		}
	}



	void loadScene(std::ifstream& file, Scene* scene);
	void loadEntity(std::ifstream& file, Scene* scene, const Entity& entity);

	Scene* oldFileLoading(const char* path) {
		std::ifstream file(path);

		if (!file.is_open()) {
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}

		std::string line;
		Scene* scene{ nullptr };

		while (std::getline(file, line)) {
			if (line.find("#SceneStart") != std::string::npos) {
				std::getline(file, line);
				std::istringstream is(line.substr(11));
				std::string name;
				is >> name;
				scene = new Scene(name);

				std::getline(file, line);
				maths::vec3 background;
				is.clear();
				is = std::istringstream(line.substr(18));
				is >> background.x >> background.y >> background.z;
				scene->setBackground(background);

				loadScene(file, scene);
			}
		}

		file.close();
		return scene;
	}

	void loadScene(std::ifstream& file, Scene* scene) {
		std::string line;

		while (std::getline(file, line)) {
			if (line.find("#EntityStart") != std::string::npos) {
				const auto& entity = scene->createEntity();
				loadEntity(file, scene, entity);
			}
			else if (line.find("#SceneEnd") != std::string::npos) {
				return;
			}
		}
	}

	void loadEntity(std::ifstream& file, Scene* scene, const Entity& entity) {
		std::string line;

		auto loadVec3 = [&line](maths::vec3& v, size_t num) {
			std::istringstream is(line.substr(num));
			is >> v.x >> v.y >> v.z;
		};

		auto loadVec3Getline = [&file, &line, &loadVec3](maths::vec3& v, size_t num) {
			std::getline(file, line);
			loadVec3(v, num);
		};

		auto loadVec4 = [&line](maths::vec4& v, size_t num) {
			std::istringstream is(line.substr(num));
			is >> v.x >> v.y >> v.z >> v.w;
		};

		auto loadVec4Getline = [&file, &line, &loadVec4](maths::vec4& v, size_t num) {
			std::getline(file, line);
			loadVec4(v, num);
		};

		auto loadString = [&line](std::string& str, size_t num) {
			std::istringstream iss(line.substr(num));
			iss >> str;
		};

		auto loadStringGetline = [&file, &line, &loadString](std::string& str, size_t num) {
			std::getline(file, line);
			loadString(str, num);
		};

		auto loadFloatGetline = [&file, &line](float& f, size_t num) {
			std::getline(file, line);
			std::istringstream is(line.substr(num));
			is >> f;
		};

		while (std::getline(file, line)) {
			if (line.find("#TagComponent") != std::string::npos) {
				auto& tag = entity.getComponent<TagComponent>();
				loadString(tag.tag, 14);
			}
			else if (line.find("#TransformComponent") != std::string::npos) {
				auto& tran = entity.getComponent<TransformComponent>();

				// #center - 7 letters
				loadVec3Getline(tran.center, 7);

				// #angles - 7 letters
				loadVec3Getline(tran.angles, 7);

				// #scale - 6  letters
				loadVec3Getline(tran.scale, 6);
			}
			else if (line.find("#RenderableComponent") != std::string::npos) {
				auto& ren = entity.addComponent<RenderableComponent>();

				if (line.find("Cube") != std::string::npos) {
					ren.name = "Cube";
					ren.vertices = MeshCreator::Cube::getVertices();
					ren.indices = MeshCreator::Cube::getIndices();
				}
				else if (line.find("Surface") != std::string::npos) {
					ren.name = "Surface";
					ren.vertices = MeshCreator::Surface::getVertices();
					ren.indices = MeshCreator::Surface::getIndices();
				}
				else if (line.find("Wall") != std::string::npos) {
					ren.name = "Wall";
					ren.vertices = MeshCreator::Wall::getVertices();
					ren.indices = MeshCreator::Wall::getIndices();
				}
				else if (line.find("Pyramid") != std::string::npos) {
					ren.name = "Pyramid";
					ren.vertices = MeshCreator::Pyramid::getVertices();
					ren.indices = MeshCreator::Pyramid::getIndices();
				}
			}
			else if (line.find("#ColorComponent") != std::string::npos) {
				auto& color = entity.addComponent<ColorComponent>();
				loadVec4(color.texture, 16);
			}
			else if (line.find("Texture2DComponent") != std::string::npos) {
				auto& texture = entity.addComponent<Texture2DComponent>();
				loadString(texture.texture, 19);
			}
			else if (line.find("#TextureCubemapComponent") != std::string::npos) {
				auto& cubemap = entity.addComponent<TextureCubemapComponent>();
				loadString(cubemap.texture, 25);
			}
			else if (line.find("#LightComponent") != std::string::npos) {
				auto& light = entity.addComponent<LightComponent>();

				// #ambientlight - 13
				loadVec4Getline(light.ambient, 13);

				// #diffuselight - 13
				loadVec4Getline(light.diffuse, 13);

				// #specularlight - 14
				loadVec4Getline(light.specular, 14);

				// #constant - 9
				loadFloatGetline(light.constant, 9);

				// #linear - 7
				loadFloatGetline(light.linear, 7);

				// #quadratic - 10
				loadFloatGetline(light.quadratic, 10);

				// #shininess - 10
				loadFloatGetline(light.shininess, 10);
			}
			else if (line.find("#CameraComponent") != std::string::npos) {
				auto& cam = entity.addComponent<CameraComponent>();

				// #id - 3
				loadStringGetline(cam.id, 3);

				// #used - 5
				std::string type;
				loadStringGetline(type, 5);

				if (type.find("perspective") != std::string::npos) { cam.Perspective = true; }
				else { cam.Perspective = false; }

				// #type - 5
				std::getline(file, line);

				// #fov - 4
				loadFloatGetline(cam.p_fov, 4);

				// #aspectratio - 12
				loadFloatGetline(cam.p_aspectRatio, 12);

				// #near - 5
				loadFloatGetline(cam.p_near, 5);

				// #far - 4
				loadFloatGetline(cam.p_far, 4);

				// #type - 5
				std::getline(file, line);

				// #left - 5
				loadFloatGetline(cam.o_left, 5);

				// #right - 6
				loadFloatGetline(cam.o_right, 6);

				// #top - 4
				loadFloatGetline(cam.o_top, 4);

				// #bottom - 7
				loadFloatGetline(cam.o_bottom, 7);

				// #near - 5
				loadFloatGetline(cam.o_near, 5);

				// #far
				loadFloatGetline(cam.o_far, 4);
			}
			else if (line.find("#ScriptComponent") != std::string::npos) {
				auto& script = entity.addComponent<PythonScriptComponent>();
				loadString(script.script, 17);
			}
			else if (line.find("#EntityEnd") != std::string::npos) {
				return;
			}
		}
	}


}
