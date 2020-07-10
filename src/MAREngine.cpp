/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"


namespace mar {
    namespace engine {


		MAREngine MAREngine::s_instance;


		void MAREngine::initialize() {
			MAR_LOG_INIT();

			m_usegui = true;

			storage::factory = m_factory;
			storage::usegui = m_usegui;

			window::Window::getInstance().initialize(settings::height, settings::width, settings::name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			auto m_framebuffer = storage::factory->createFrameBuffer();
			m_framebuffer->initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			auto camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initialize();
			if (storage::usegui)
				camera_layer->set(m_framebuffer->getSpecification().width, m_framebuffer->getSpecification().height);
			else
				camera_layer->set(window::Window::getInstance().getWidth(), window::Window::getInstance().getHeight());
			camera_layer->mouseSetup();

			auto gui_layer = new layers::GUILayer("Default GUI Layer");
			gui_layer->initialize();
			gui_layer->set(m_framebuffer);

			layers::LayerStack m_stack;
			m_stack.pushOverlay(gui_layer);
			m_stack.pushLayer(camera_layer);

			filesystem::fnc::loadSceneFromFile(m_pathLoad);
			assignLoadedLayers(&m_stack, gui_layer, camera_layer, m_framebuffer);

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (window::Window::getInstance().shouldClose() && !m_shouldRestart) 
			{
				window::Window::getInstance().clearScreen();
				graphics::Renderer::getStatistics().resetStatistics();

				if (storage::usegui)
					camera_layer->set(m_framebuffer->getSpecification().width, m_framebuffer->getSpecification().height);
				else
					camera_layer->set(window::Window::getInstance().getWidth(), window::Window::getInstance().getHeight());
				
				m_stack.update();

				window::Window::getInstance().swapBuffers();
			}
			// --------------- RENDER LOOP -------------------- //
			//////////////////////////////////////////////////////

			m_framebuffer->close();
			m_stack.close();
		}

		void MAREngine::exit() {
		start_again:
			if (m_shouldRestart) {
				m_shouldRestart = false;
				run();
				goto start_again;
			}

			window::Window::getInstance().shutdown();
		}

		void MAREngine::assignLoadedLayers(	layers::LayerStack* stack, layers::GUILayer* gui_layer,
											layers::CameraLayer* camera_layer, Ref<graphics::FrameBuffer>& framebuffer) {
			using fs = filesystem::fnc;

			if (fs::shouldLoadMeshes()) {
				if (fs::getMeshCount() + 1 != fs::getMeshTypes().size()) {
					MAR_CORE_ERROR("Mesh Types size is not equal to mesh_count!");
					return;
				}

				for (size_t i = 0; i < fs::getMeshTypes().size(); i++) {
					auto layer = new layers::MeshLayer("Mesh Layer");
					layer->initialize();
					layer->getMesh()->setType(fs::getMeshTypes()[i]);

					for (unsigned int j = 0; j < fs::getShapes()[i].size(); j++) {
						auto shape = fs::getShapes()[i][j];
						auto center = fs::getCenters()[i][j];
						auto angle = fs::getAngles()[i][j];
						auto scale = fs::getScales()[i][j];
						auto color = fs::getColors()[i][j];
						auto texture = fs::getTextures()[i][j];

						if (layer->getMesh()->getMeshType() == OBJECTS_MESH_TYPE) {
							auto obj = fs::getObjs()[i][j];
							shape->assignDataFromFile(obj.c_str());
						}
							
						shape->setDefaultColor(color);
						layer->getMesh()->submitShape(shape, center, angle, scale, texture.c_str());
					}

					layer->load();
					layer->set(framebuffer);

					gui_layer->submit(layer->getMesh());
					stack->pushLayer(layer);
				}

				MAR_CORE_INFO("Scene is loaded from meshes!");
			}
			else if (fs::shouldLoadScene()) {
				if (fs::getMeshTypes().size() != fs::getSceneTypes().size()) {
					MAR_CORE_ERROR("Loaded types of mesh are not equal to the size of scene types size!");
					return;
				}

				for (unsigned int i = 0; i < fs::getMeshTypes().size(); i++) {
					auto layer = new layers::MeshLayer("Mesh Layer");
					layer->initialize();
					layer->scene(fs::getSceneTypes()[i], fs::getMeshTypes()[i]);
					layer->set(framebuffer);
					stack->pushLayer(layer);
					gui_layer->submit(layer->getMesh());
				}

				MAR_CORE_INFO("Default scene is being loaded!");
			}
			else {
				MAR_CORE_ERROR("Unsupported Data!");
				return;
			}
		}


} }