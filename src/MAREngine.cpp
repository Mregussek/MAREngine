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

			storage::usegui = true;

			window::Window::getInstance().initialize(settings::height, settings::width, settings::name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			layers::LayerStack m_stack;

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

			m_stack.pushLayer(camera_layer);
			m_stack.pushOverlay(gui_layer);
			
			auto entitylayer = new layers::EntityLayer("Entity Layer");
			entitylayer->getScene()->setName("Default Scene");
			
			// ADD ENTITIES
			{
				// FIRST ENTITY
				{
					auto entity = entitylayer->getScene()->createEntity();

					auto& tag = entity.getComponent<ecs::TagComponent>();
					tag = std::string("First Entity");

					auto& tran = entity.getComponent<ecs::TransformComponent>();
					tran.scale = { 1.f, 1.f, 1.f };
					tran.angles = { 65.f, 0.f, 0.f };
					tran.center = { 0.f, 2.f, 1.f };
					ecs::System::handleTransformComponent(tran);

					auto& ren = entity.addComponent<ecs::RenderableComponent>();
					ren.vertices = graphics::MeshCreator::getVertices_Cube();
					ren.indices = graphics::MeshCreator::getIndices_Cube();

					entity.addComponent<ecs::ColorComponent>(maths::vec3{ 0.2f, 0.5f, 0.2f });
				}

				// SECOND ENTITY
				{
					auto entity = entitylayer->getScene()->createEntity();

					auto& tag = entity.getComponent<ecs::TagComponent>();
					tag = std::string("Second Entity");

					auto& ren = entity.addComponent<ecs::RenderableComponent>();
					ren.vertices = graphics::MeshCreator::getVertices_Pyramid();
					ren.indices = graphics::MeshCreator::getIndices_Pyramid();

					auto& tran = entity.getComponent<ecs::TransformComponent>();
					tran.scale = { 1.f, 1.5f, 1.f };
					tran.angles = { 0.f, 25.f, 0.f };
					tran.center = { -3.f, 2.f, -1.f };
					ecs::System::handleTransformComponent(tran);

					entity.addComponent<ecs::ColorComponent>(maths::vec3{ 0.5f, 0.9f, 0.25f });
				}

				// THIRD ENTITY
				{
					auto entity = entitylayer->getScene()->createEntity();

					auto& tag = entity.getComponent<ecs::TagComponent>();
					tag = std::string("Third Entity");

					auto& ren = entity.addComponent<ecs::RenderableComponent>();
					ren.vertices = graphics::MeshCreator::getVertices_Surface();
					ren.indices = graphics::MeshCreator::getIndices_Surface();

					auto& tran = entity.getComponent<ecs::TransformComponent>();
					tran.scale = { 1.f, 1.f, 1.f };
					tran.angles = { 0.f, 0.f, 0.f };
					tran.center = { 0.f, 0.f, 0.f };
					ecs::System::handleTransformComponent(tran);

					entity.addComponent<ecs::ColorComponent>(maths::vec3{ 0.5f, 0.2f, 0.9f });
				}

				// FOURTH ENTITY
				{
					auto entity = entitylayer->getScene()->createEntity();

					auto& tag = entity.getComponent<ecs::TagComponent>();
					tag = std::string("Fourth Entity");

					auto& ren = entity.addComponent<ecs::RenderableComponent>();
					ren.vertices = graphics::MeshCreator::getVertices_Wall();
					ren.indices = graphics::MeshCreator::getIndices_Wall();

					auto& tran = entity.getComponent<ecs::TransformComponent>();
					tran.scale = { 1.f, 1.f, 1.f };
					tran.angles = { 0.f, 25.f, 0.f };
					tran.center = { -2.f, 0.f, 0.f };
					ecs::System::handleTransformComponent(tran);

					entity.addComponent<ecs::ColorComponent>(maths::vec3{ 0.9f, 0.2f, 0.3f });
				}

				// FIFTH ENTITY
				{
					auto entity = entitylayer->getScene()->createEntity();

					auto& tag = entity.getComponent<ecs::TagComponent>();
					tag = std::string("Light Entity");

					auto& ren = entity.addComponent<ecs::RenderableComponent>();
					ren.vertices = graphics::MeshCreator::getVertices_Cube();
					ren.indices = graphics::MeshCreator::getIndices_Cube();

					auto& tran = entity.getComponent<ecs::TransformComponent>();
					tran.scale = { 0.5f, 0.5f, 0.5f };
					tran.angles = { 0.f, 0.f, 0.f };
					tran.center = { 0.f, 0.f, 5.0f };
					ecs::System::handleTransformComponent(tran);

					entity.addComponent<ecs::ColorComponent>(maths::vec3{ 1.f, 1.f, 1.f });
					entity.addComponent<ecs::LightComponent>();
				}
			}
			
			entitylayer->initialize();
			m_stack.pushLayer(entitylayer);
			gui->submit(entitylayer->getScene());

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (window::Window::getInstance().shouldClose() && !m_shouldRestart) 
			{
				window::Window::getInstance().clearScreen();

				if (storage::usegui) {
					m_framebuffer.bind();
					m_framebuffer.clear();

					entitylayer->getRenderer()->submit(entitylayer->getScene());

					m_stack.update();

					graphics::RendererEntity::clearStatistics();
				}
				else {
					m_stack.update();
				}

				window::Window::getInstance().swapBuffers();
			}
			// --------------- RENDER LOOP -------------------- //
			//////////////////////////////////////////////////////

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