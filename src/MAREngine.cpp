/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"


namespace mar {
    namespace engine {


		const char MAREngineSettings::name[10] = "MAREngine";
		const int MAREngineSettings::width{ 1500 };
		const int MAREngineSettings::height{ 900 };
		const char MAREngineSettings::portName[10] = "\\\\.\\COM7";
		const char* MAREngineSettings::glsl_version = "#version 330";

		MAREngine MAREngine::s_instance;


		void MAREngine::initialize() {
			MAR_LOG_INIT();

			m_usegui = true;

			filesystem::Storage::getInstance()->setup(m_factory, m_usegui);

			m_window = window::Window();

			m_window.initialize(MAREngineSettings::height, MAREngineSettings::width, MAREngineSettings::name);
			window::Input::initialize(m_window.getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			gui::GUI m_gui;
			m_gui.initialize(&m_window, MAREngineSettings::glsl_version, m_usegui);

			auto m_framebuffer = m_factory->createFrameBuffer();
			m_framebuffer->initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			auto camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initializeLayer(new graphics::Camera());
			if(m_usegui) camera_layer->set(&m_framebuffer->getSpecification().width, &m_framebuffer->getSpecification().height);
			else camera_layer->set((const float*)&MAREngineSettings::width, (const float*)&MAREngineSettings::height);
			camera_layer->set(&m_window);

			auto gui_layer = new layers::GUILayer("Default GUI Layer");
			gui_layer->initializeLayer(&m_gui);
			gui_layer->set(&graphics::Renderer::getStatistics(), m_framebuffer);

			layers::LayerStack m_stack;
			m_stack.pushOverlay(gui_layer);
			m_stack.pushLayer(camera_layer);

			filesystem::fnc::loadSceneFromFile(m_pathLoad);
			assignLoadedLayers(&m_stack, gui_layer, camera_layer, m_framebuffer);

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (m_window.shouldClose() && !m_shouldRestart) 
			{
				m_window.clearScreen();
				graphics::Renderer::getStatistics().resetStatistics();
				m_stack.update();
				m_window.swapBuffers();

				MAR_CORE_CHECK_FOR_ERROR();
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

			m_window.shutdown();
		}

		void MAREngine::assignLoadedLayers(	layers::LayerStack* stack,
											layers::GUILayer* gui_layer,
											layers::CameraLayer* camera_layer,
											Ref<graphics::FrameBuffer>& framebuffer) {
			using fs = filesystem::fnc;
			using sto = filesystem::Storage;

			if (fs::getMeshCount() == MESHES_LOADER) {
				if (fs::getMeshCount() != fs::getMeshTypes().size() + 1) {
					MAR_CORE_ERROR("Mesh Types size is not equal to mesh_count!");
					return;
				}

				for (int i = 0; i < fs::getMeshCount() + 1; i++) {
					std::string name = "Mesh Layer " + std::to_string(i);
					auto layer = new layers::MeshLayer(name.c_str());
					layer->initializeLayer(new graphics::Renderer(), new graphics::Mesh());
					layer->create(sto::getInstance()->factory, sto::getInstance()->usegui);
					layer->getMesh()->setType(fs::getMeshTypes()[i]);

					for (unsigned int j = 0; j < fs::getShapes()[i].size(); j++) {
						if (layer->getMesh()->getMeshType() == OBJECTS_MESH_TYPE) {
							fs::getShapes()[i][j]->assignDataFromFile(fs::getObjs()[i][j].c_str());
						}

						fs::getShapes()[i][j]->setDefaultColor(fs::getColors()[i][j]);
						layer->getMesh()->submitShape(fs::getShapes()[i][j], fs::getCenters()[i][j],
							fs::getAngles()[i][j], fs::getScales()[i][j], fs::getTextures()[i][j].c_str());
					}

					layer->load();
					layer->set(&gui::GUI::getGUIData(), &camera_layer->getCameraData());
					layer->set(framebuffer);

					gui_layer->submit(layer->getMesh());
					stack->pushLayer(layer);
				}

				MAR_CORE_INFO("Scene is loaded from meshes!");
			}
			else if (fs::getSceneCount() == SCENES_LOADER) {
				if (fs::getMeshTypes().size() != fs::getSceneTypes().size()) {
					MAR_CORE_ERROR("Loaded types of mesh are not equal to the size of scene types size!");
					return;
				}

				for (unsigned int i = 0; i < fs::getMeshTypes().size(); i++) {
					std::string name = "Mesh Layer " + std::to_string(i);
					layers::MeshLayer* layer = new layers::MeshLayer(name.c_str());
					layer->initializeLayer(new graphics::Renderer(), new graphics::Mesh());
					layer->create(sto::getInstance()->factory, sto::getInstance()->usegui);
					layer->scene(fs::getSceneTypes()[i], fs::getMeshTypes()[i]);
					layer->set(&gui::GUI::getGUIData(), &camera_layer->getCameraData());
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