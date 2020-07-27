/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"
#include "Core/Scene/Scene.h"


namespace mar {
    namespace engine {


		MAREngine MAREngine::s_instance;


		void MAREngine::initialize() {
			MAR_LOG_INIT();

			storage::usegui = true;

			window::Window::getInstance().initialize(settings::height, settings::width, settings::name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			layers::LayerStack m_stack;
			graphics::Light m_light;

			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			m_framebuffer.initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			auto camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initialize();
			if (storage::usegui)
				camera_layer->getCamera()->setWindowSize(&m_framebuffer.getSpecification().width, &m_framebuffer.getSpecification().height);
			else
				camera_layer->getCamera()->setWindowSize((const float*)&window::Window::getInstance().getWidth(), (const float*)&window::Window::getInstance().getHeight());
			camera_layer->mouseSetup();

			auto gui_layer = new layers::LayerGUI("Default GUI Layer");
			gui_layer->initialize();
			auto gui = gui_layer->getGUIInstance();
			gui->set(m_framebuffer);
			gui->set(&m_light);

			m_stack.pushLayer(camera_layer);
			m_stack.pushOverlay(gui_layer);

			/*
			
			editor::filesystem::loadSceneFromFile(m_pathLoad);
			if (auto loaded = editor::filesystem::assignLoadedLayers())
				for (uint32_t i = 0; i < loaded->size(); i++) {
					loaded->at(i)->getRenderer()->setLight(&m_light);
					gui->submit(loaded->at(i)->getMesh());
					m_stack.pushLayer(loaded->at(i));
				}
			*/

			ecs::Scene scene;
			auto entity = scene.createEntity();

			{
				auto& ren = entity.getComponent<ecs::RenderableComponent>();
				ren.vertices = graphics::MeshCreator::getVertices_Cube();
				ren.indices = graphics::MeshCreator::getIndices_Cube();

				auto& tran = entity.getComponent<ecs::TransformComponent>();
				tran.scale = maths::mat4::scale({ 1.f, 1.f, 1.f });
				tran.translation = maths::mat4::translation({ 0.f, 2.f, 1.f });
				tran.rotation = maths::mat4::rotation(maths::Trig::toRadians(65.f), { 1.f, 0.f, 0.f });
				tran = tran.scale * tran.translation * tran.rotation;

				entity.addComponent<ecs::ColorComponent>(maths::vec3{ 0.2f, 0.5f, 0.2f });
			}
			
			auto secentity = scene.createEntity();

			{
				auto& secren = secentity.getComponent<ecs::RenderableComponent>();
				secren.vertices = graphics::MeshCreator::getVertices_Pyramid();
				secren.indices = graphics::MeshCreator::getIndices_Pyramid();

				auto& sectran = secentity.getComponent<ecs::TransformComponent>();
				sectran.scale = maths::mat4::scale({ 1.f, 1.5f, 1.f });
				sectran.translation = maths::mat4::translation({ -3.f, 2.f, -1.f });
				sectran.rotation = maths::mat4::rotation(maths::Trig::toRadians(25.f), { 0.f, 1.f, 0.f });
				sectran = sectran.scale * sectran.translation * sectran.rotation;

				secentity.addComponent<ecs::ColorComponent>(maths::vec3{ 0.5f, 0.9f, 0.25f });
			}
			
			graphics::RendererEntity entrenderer;
			entrenderer.initialize();
			entrenderer.setMVP(&graphics::Camera::getCameraData().mvp);

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (window::Window::getInstance().shouldClose() && !m_shouldRestart) 
			{
				window::Window::getInstance().clearScreen();
				graphics::RendererOpenGL::getStatistics().resetStatistics();

				if (storage::usegui) {
					m_framebuffer.bind();
					m_framebuffer.clear();

					entrenderer.submit(entity);
					entrenderer.submit(secentity);
					entrenderer.update();
					entrenderer.clear();

					m_stack.update();
				}
				else {
					m_stack.update();
				}

				window::Window::getInstance().swapBuffers();
			}
			// --------------- RENDER LOOP -------------------- //
			//////////////////////////////////////////////////////

			entrenderer.close();
			m_framebuffer.close();
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


} }