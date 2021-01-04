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


#include "ViewportWidget.h"
#include "../EntityPanels/EntityWidgetPanel.h"
#include "../../EditorLogging.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/events/RenderEvents.h"
#include "../../../Core/ecs/Entity/EventsComponentEntity.h"


namespace marengine {


	void WViewportWidget::create() {
		constexpr float xDefault = 800.f;
		constexpr float yDefault = 600.f;
		m_framebuffer.initialize(xDefault, yDefault);
		updateAspectRatio();

		m_camera.initialize(m_aspectRatio);

		FEventsComponentEntity::Instance->onEditorCameraSet(m_camera.getCameraData());
	}

	void WViewportWidget::destroy() {
		m_framebuffer.close();
	}

	void WViewportWidget::beginFrame() {
		m_framebuffer.unbind();
	}

	void WViewportWidget::updateFrame() {
		displayViewportControlPanel();

		auto& style{ ImGui::GetStyle() };
		const auto rememberDefaultVal{ style.WindowPadding };
		style.WindowPadding = ImVec2(1.f, 1.f);

		ImGui::Begin("ViewPort", nullptr);

		displayActualViewport();
		handleGuizmo();

		ImGui::End();

		style.WindowPadding = rememberDefaultVal;

		EDITOR_TRACE("GUI: Displaying viewport");
	}

	void WViewportWidget::displayViewportControlPanel() {
		auto displayEditorModeButtons = [&sceneManager = SceneManager::Instance, &camera = std::as_const(m_camera)]() {
			if (ImGui::Button("PLAY")) { sceneManager->setPlayMode(); }
			if (ImGui::Checkbox("UseCameraEditor", &sceneManager->useEditorCamera)) {
				if (sceneManager->isEditorMode()) {
					if (sceneManager->useEditorCamera) {
						FEventsComponentEntity::Instance->onEditorCameraSet(camera.getCameraData());
					}
					else {
						FEventsComponentEntity::Instance->onGameCameraSet();
					}
				}
			}
		};

		auto displayPlayModeButtons = [&sceneManager = SceneManager::Instance, &camera = std::as_const(m_camera)]() {
			if (ImGui::Button("STOP")) {
				sceneManager->setExitPlayMode();
				if (sceneManager->useEditorCamera) {
					FEventsComponentEntity::Instance->onEditorCameraSet(camera.getCameraData());
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

		ImGui::Begin("Viewport Control Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (SceneManager::Instance->isEditorMode()) { displayEditorModeButtons(); }
			else { displayPlayModeButtons(); }

			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	void WViewportWidget::displayActualViewport() {
		const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		const auto size = m_framebuffer.getSize();

		if (size.x != viewportSize.x || size.y != viewportSize.y) {
			m_framebuffer.resize(viewportSize.x, viewportSize.y);
			updateAspectRatio();
		}

		const uint32_t id = m_framebuffer.getColorAttach();
		ImGui::Image((ImTextureID)id, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void WViewportWidget::handleGuizmo() {
		if (SceneManager::Instance->useEditorCamera) {
			const auto& currentEntity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
			const bool entityExists = &currentEntity != nullptr;
			m_guizmo.selectType();

			if (SceneManager::Instance->isEditorMode()) {
				if (entityExists) { m_guizmo.draw(m_camera, currentEntity); }

				bool useInputInCamera = false;
				if (ImGui::IsWindowFocused()) { useInputInCamera = true; }
				if (m_camera.update(m_aspectRatio, useInputInCamera)) {
					RenderEvents::Instance().onMainCameraUpdate(*m_camera.getCameraData());
				}
			}
		}
	}

	void WViewportWidget::bind(maths::vec3 backgroundColor) const {
		m_framebuffer.bind();
		m_framebuffer.clear(backgroundColor);
	}

	void WViewportWidget::unbind() const {
		m_framebuffer.unbind();
	}

	void WViewportWidget::updateAspectRatio() {
		const auto size = m_framebuffer.getSize();
		m_aspectRatio = size.x / size.y;
	}


}