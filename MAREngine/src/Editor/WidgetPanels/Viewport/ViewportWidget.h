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


#ifndef MAR_ENGINE_W_VIEWPORT_WIDGET_H
#define MAR_ENGINE_W_VIEWPORT_WIDGET_H


#include "../IWidgetPanel.h"
#include "../../Camera/Camera.h"
#include "GuizmoWidget.h"
#include "../../../Platform/OpenGL/FramebufferOpenGL.h"


namespace marengine {

	class Entity; 
	class EntityCollection;


	class WViewportWidget : public IWidgetPanel {
	public:

		virtual void create() override;
		virtual void destroy() override;

		virtual void beginFrame() override;
		virtual void updateFrame() override;

		void bind(maths::vec3 backgroundColor) const;

	private:

		void unbind() const;

		void updateAspectRatio();

		void displayViewportControlPanel();
		void displayActualViewport();
		void handleGuizmo();

		FramebufferOpenGL m_framebuffer;
		Camera m_camera;
		float m_aspectRatio{ 1.33f };
		GUI_Guizmo m_guizmo;

	};


}


#endif // !MAR_ENGINE_W_VIEWPORT_WIDGET_H