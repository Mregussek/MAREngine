/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "CameraLayer.h"


namespace mar {
	namespace layers {


		CameraLayer::CameraLayer(const char* name) 
			: m_debugName(name) ,
			m_camera(nullptr)
		{}

		void CameraLayer::initialize() {
			m_camera = new graphics::Camera();

			m_camera->initialize();
		}

		void CameraLayer::update() {
			m_camera->processInput();
			m_camera->updateData();
		}

		void CameraLayer::closeLayer() {
			delete m_camera;
		}

		void CameraLayer::mouseSetup() {
			auto win = &window::Window::getInstance();

			m_camera->setMouseCall(&win->getMouseX(), &win->getMouseY());
			m_camera->setScrollCall(&win->getScrollX(), &win->getScrollY());
		}


} }