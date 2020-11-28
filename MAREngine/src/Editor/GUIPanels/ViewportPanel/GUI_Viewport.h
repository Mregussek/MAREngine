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


#ifndef MAR_ENGINE_EDITOR_GUI_VIEWPORT_H
#define MAR_ENGINE_EDITOR_GUI_VIEWPORT_H


#include "../../../mar.h"
#include "../../Camera/Camera.h"
#include "../../../Platform/OpenGL/FramebufferOpenGL.h"


namespace mar::ecs { class SceneManager; }
namespace mar::editor {


	class GUI_Viewport {
	public:

		void initialize();
		void close();

		void display(ecs::SceneManager* sceneManager);

		void bind(maths::vec3 backgroundColor) const;
		void unbind() const;

		void updateAspectRatio();

	private:

		void displayMainMenuBar(ecs::SceneManager* sceneManager);
		void displayActualViewport();

		platforms::FramebufferOpenGL m_framebuffer;
		Camera m_camera;
		float m_aspectRatio{ 1.33f };

	};


}


#endif // !MAR_ENGINE_EDITOR_GUI_VIEWPORT_H
