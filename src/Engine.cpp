/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Engine.h"


namespace mar {
    namespace engine {


		MAREngine MAREngine::s_instance;


		void MAREngine::initWindow(const int& height, const int& width, const char* name) {
			MAR_LOG_INIT();

			window::Window::getInstance().initialize(height, width, name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::closeWindow() {
			window::Window::getInstance().shutdown(); 
		}

		void MAREngine::clearWindowScreen() {
			window::Window::getInstance().clearScreen(); 
		}

		void MAREngine::swapWindowBuffers() {
			window::Window::getInstance().swapBuffers(); 
		}

		const bool MAREngine::shouldWindowClose() { 
			return window::Window::getInstance().shouldClose(); 
		}

		void MAREngine::updateBackground(editor::GUI* gui, ecs::Scene* scene) {
			auto& back = scene->getBackground();
			window::Window::getInstance().updateBackgroundColor(back);

			if (gui) {
				auto& frame = gui->getFramebuffer();
				auto& spec = frame.getSpecification();
				spec.backgroundColor = back;
			}
		}

		void MAREngine::updateBackground(ecs::Scene* scene) {
			auto& back = scene->getBackground();
			window::Window::getInstance().updateBackgroundColor(back);
		}

		editor::GUI MAREngine::createGUI() {
			return editor::GUI();
		}

		editor::Camera MAREngine::createGUICamera() {
			return editor::Camera();
		}

		layers::LayerStack MAREngine::createLayerStack() {
			return layers::LayerStack(); 
		}

		layers::EntityLayer* MAREngine::createEntityLayer() {
			return new layers::EntityLayer("Entity Layer"); 
		}

		layers::LayerGUI* MAREngine::createEditorLayer() {
			return new layers::LayerGUI("Default GUI Layer"); 
		}

		ecs::Scene* MAREngine::loadSceneFromFile() {
			return editor::Filesystem::openFile(m_pathLoad.c_str());
		}


} }