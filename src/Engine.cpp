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

		void MAREngine::submitSceneToGUI(layers::LayerGUI* gui_layer, ecs::Scene* scene) {
			gui_layer->getGUIInstance()->submit(scene);
		}

		graphics::FrameBuffer<graphics::FrameBufferOpenGL>& MAREngine::getFramebuffer(layers::LayerGUI* guilayer) {
			return guilayer->getGUIInstance()->getFramebuffer();
		}


} }