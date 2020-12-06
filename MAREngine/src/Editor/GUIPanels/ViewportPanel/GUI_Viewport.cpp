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


#include "GUI_Viewport.h"
#include "../../EditorLogging.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/ecs/SceneEvents.h"
#include "GUI_Guizmo.h"


namespace mar::editor {


	void GUI_Viewport::initialize() {
		constexpr float xDefault = 800.f;
		constexpr float yDefault = 600.f;
		m_framebuffer.initialize(xDefault, yDefault);
		updateAspectRatio();

		m_camera.initialize(m_aspectRatio);

		ecs::SceneEvents::Instance().onEditorCameraSet(m_camera.getCameraData());
	}

	void GUI_Viewport::close() {
		m_framebuffer.close();
	}

	void GUI_Viewport::display(ecs::SceneManager* sceneManager, const ecs::EntityCollection& currentCollection, const ecs::Entity& currentEntity) {
		ImGui::Begin("Viewport Control Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
		
		displayMainMenuBar(sceneManager);

		ImGui::End();

		ImGuiStyle* style = &ImGui::GetStyle();
		const auto rememberDefaultVal = style->WindowPadding;
		style->WindowPadding = ImVec2(1.f, 1.f);

		ImGui::Begin("ViewPort", nullptr);

		displayActualViewport();

		if (sceneManager->useEditorCamera) {
			const bool entityExists = &currentEntity != nullptr;
			const bool collectionExists = &currentCollection != nullptr;
			m_guizmo.selectType();

			if (collectionExists) { 
				m_guizmo.draw(m_camera, currentCollection);
			}
			else if (entityExists) { 
				m_guizmo.draw(m_camera, currentEntity); 
			}

			if (ImGui::IsWindowFocused()) {
				m_camera.update(m_aspectRatio);
			}
		}
		
		ImGui::End();

		style->WindowPadding = rememberDefaultVal;

		EDITOR_TRACE("GUI: Displaying viewport");
	}

	void GUI_Viewport::displayMainMenuBar(ecs::SceneManager* sceneManager) {
		auto displayEditorModeButtons = [&sceneManager, this]() {
			if (ImGui::Button("PLAY")) { sceneManager->setPlayMode(); }
			if (ImGui::Checkbox("UseCameraEditor", &sceneManager->useEditorCamera)) {
				if (sceneManager->isEditorMode()) {
					if (sceneManager->useEditorCamera) {
						ecs::SceneEvents::Instance().onEditorCameraSet(m_camera.getCameraData());
					}
					else {
						ecs::SceneEvents::Instance().onGameCameraSet();
					}
				}
			}
		};

		auto displayPlayModeButtons = [&sceneManager, this]() {
			if (ImGui::Button("STOP")) {
				sceneManager->setExitPlayMode();
				if (sceneManager->useEditorCamera) {
					ecs::SceneEvents::Instance().onEditorCameraSet(m_camera.getCameraData());
				}
			}

			ImGui::SameLine();

			if (!sceneManager->isPauseMode()) {
				if (ImGui::Button("PAUSE")) { sceneManager->setPauseMode(); }
			}
			else {
				if (ImGui::Button("RESUME")) { sceneManager->unsetPauseMode(); }
			}
		};

		if (ImGui::BeginMenuBar()) {
			if (sceneManager->isEditorMode()) { displayEditorModeButtons(); }
			else { displayPlayModeButtons(); }

			ImGui::EndMenuBar();
		}
	}

	void GUI_Viewport::displayActualViewport() {
		const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		const auto size = m_framebuffer.getSize();

		if (size.x != viewportSize.x || size.y != viewportSize.y) {
			m_framebuffer.resize(viewportSize.x, viewportSize.y);
			updateAspectRatio();
		}

		const uint32_t id = m_framebuffer.getColorAttach();

		ImGui::Image((ImTextureID)id, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void GUI_Viewport::bind(maths::vec3 backgroundColor) const {
		m_framebuffer.bind();
		m_framebuffer.clear(backgroundColor);
	}

	void GUI_Viewport::unbind() const {
		m_framebuffer.unbind();
	}

	void GUI_Viewport::updateAspectRatio() {
		const auto size = m_framebuffer.getSize();
		m_aspectRatio = size.x / size.y;
	}


}