/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "ViewportImGuiWidget.h"
#include "InspectorImGuiWidget.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Core/ecs/Entity/EventsCameraEntity.h"


namespace marengine {


    void FViewportImGuiWidget::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pSceneManagerEditor = serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        m_pInspectorWidget = serviceLocator->retrieve<FInspectorImGuiWidget>();

        constexpr float xDefault = 800.f;
        constexpr float yDefault = 600.f;
        m_framebuffer.initialize(xDefault, yDefault);
        updateAspectRatio();

        m_camera.initialize(m_aspectRatio);

        if(m_pSceneManagerEditor->usingEditorCamera()) {
            FEventsCameraEntity::onEditorCameraSet(m_camera.getCameraData());
        }
    }

    void FViewportImGuiWidget::destroy() {
        m_framebuffer.close();
    }

    void FViewportImGuiWidget::beginFrame() {
        m_framebuffer.unbind();
    }

    void FViewportImGuiWidget::updateFrame() {
        displayViewportControlPanel();

        auto& style{ ImGui::GetStyle() };
        const auto rememberDefaultVal{ style.WindowPadding };
        style.WindowPadding = ImVec2(1.f, 1.f);

        ImGui::Begin("ViewPort", nullptr);

        displayActualViewport();
        handleGuizmo();

        ImGui::End();

        style.WindowPadding = rememberDefaultVal;
    }

    void FViewportImGuiWidget::displayViewportControlPanel() {
        auto displayEditorModeButtons = [this]() {
            if (ImGui::Button("PLAY")) { m_pSceneManagerEditor->setPlayMode(); }
            ImGui::SameLine();
            if (m_pSceneManagerEditor->usingEditorCamera()) {
                if (ImGui::Button("Use Game Camera")) {
                    m_pSceneManagerEditor->useGameCamera();
                    FEventsCameraEntity::onGameCameraSet();
                }
            }
            else {
                if (ImGui::Button("Use Editor Camera")) {
                    m_pSceneManagerEditor->useEditorCamera();
                    FEventsCameraEntity::onEditorCameraSet(m_camera.getCameraData());
                }
            }
        };

        auto displayPlayModeButtons = [this]() {
            if (ImGui::Button("STOP")) {
                m_pSceneManagerEditor->setExitPlayMode();
                if (m_pSceneManagerEditor->usingEditorCamera()) {
                    FEventsCameraEntity::onEditorCameraSet(m_camera.getCameraData());
                }
            }
            ImGui::SameLine();
            if (m_pSceneManagerEditor->isPauseMode()) {
                if (ImGui::Button("RESUME")) {
                    m_pSceneManagerEditor->setExitPauseMode();
                }

            }
            else {
                if (ImGui::Button("PAUSE")) {
                    m_pSceneManagerEditor->setPauseMode();
                }
            }
        };

        ImGui::Begin("Viewport Control Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (m_pSceneManagerEditor->isEditorMode()) {
                displayEditorModeButtons();
            }
            else {
                displayPlayModeButtons();
            }

            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    void FViewportImGuiWidget::displayActualViewport() {
        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        const auto size = m_framebuffer.getSize();

        if (size.x != viewportSize.x || size.y != viewportSize.y) {
            m_framebuffer.resize(viewportSize.x, viewportSize.y);
            updateAspectRatio();
        }

        const uint32_t id = m_framebuffer.getColorAttach();
        ImGui::Image((ImTextureID)id, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }

    void FViewportImGuiWidget::handleGuizmo() {
        if (m_pSceneManagerEditor->usingEditorCamera()) {
            // TODO: bring back imguizmo
            //const auto& currentEntity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
            //const bool entityExists = &currentEntity != nullptr;
            m_guizmo.selectType();

            if (FSceneManagerEditor::Instance->isEditorMode()) {
                if (m_pInspectorWidget->isInspectedEntityValid()) {
                    m_guizmo.draw(m_camera, m_pInspectorWidget->getInspectedEntity());
                }

                const bool cameraWasRecalculated{ m_camera.update(m_aspectRatio, ImGui::IsWindowFocused()) };
                if (cameraWasRecalculated) {
                    FEventsCameraEntity::onMainCameraUpdate(m_camera.getCameraData());
                }
            }
        }
    }

    void FViewportImGuiWidget::bind(maths::vec3 backgroundColor) const {
        m_framebuffer.bind();
        m_framebuffer.clear(backgroundColor);
    }

    void FViewportImGuiWidget::unbind() const {
        m_framebuffer.unbind();
    }

    void FViewportImGuiWidget::updateAspectRatio() {
        const auto size = m_framebuffer.getSize();
        m_aspectRatio = size.x / size.y;
    }


}
