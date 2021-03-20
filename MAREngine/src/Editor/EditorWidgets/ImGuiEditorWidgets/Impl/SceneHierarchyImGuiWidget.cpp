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


#include "SceneHierarchyImGuiWidget.h"
#include "InspectorImGuiWidget.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../Events/EventsEntityImGuiWidget.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Window/IWindow.h"


namespace marengine {


    void FSceneHierarchyImGuiWidget::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pSceneManagerEditor = serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        m_pWindow = serviceLocator->retrieve<FImGuiTypeHolder<IWindow*>>()->pInstance;
        m_pInspectorWidget = serviceLocator->retrieve<FInspectorImGuiWidget>();
    }

    void FSceneHierarchyImGuiWidget::updateFrame() {
        ImGui::Begin("Scene Hierarchy");

        buttonsAtPanel();

        ImGui::Text("SCENE - %s", m_pSceneManagerEditor->getScene()->getName().c_str());
        ImGui::Separator();

        const FEntityArray& entities{ m_pSceneManagerEditor->getScene()->getEntities() };
        treesFor(entities);

        popUpMenu();

        ImGui::End();
    }

    void FSceneHierarchyImGuiWidget::treesFor(const FEntityArray& entities) const {
        auto displayTreeOrMenuItemRecursively = [this](const Entity& entity) {
            constexpr ImGuiTreeNodeFlags treeNodeFlags{
                    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth
            };
            const char* entityTag{ entity.getComponent<TagComponent>().tag.c_str() };

            if (entity.hasChildren()) { // if entity has children we want tree nodes
                const bool isTreeOpen{ ImGui::TreeNodeEx(entityTag, treeNodeFlags) };
                if (ImGui::IsItemClicked()) {
                    FEventsEntityImGuiWidgets::onSelectedEntity(entity);

                }
                if (isTreeOpen) {
                    const FEntityArray& children{ entity.getChildren() };
                    treesFor(children);

                    ImGui::TreePop();
                }
            }
            else { // if not normal menu item
                if (ImGui::MenuItem(entityTag)) {
                    FEventsEntityImGuiWidgets::onSelectedEntity(entity);
                }
            }
        };

        std::for_each(entities.cbegin(), entities.cend(), displayTreeOrMenuItemRecursively);
    }

    void FSceneHierarchyImGuiWidget::buttonsAtPanel() const {
        if (ImGui::Button("+ E")) {
            FEventsEntityImGuiWidgets::onCreateEntity();
        }

        ImGui::SameLine();

        if(m_pInspectorWidget->isInspectedEntityValid()) {
            ImGui::SameLine();

            if(ImGui::Button("Copy - E")) {
                FEventsEntityImGuiWidgets::onCopyEntity(m_pInspectorWidget->getInspectedEntity());
            }

            ImGui::SameLine();

            if(ImGui::Button("- E")) {
                FEventsEntityImGuiWidgets::onDestroyEntity(m_pInspectorWidget->getInspectedEntity());
            }
        }
    }

    void FSceneHierarchyImGuiWidget::popUpMenu() const {
        if (m_pSceneManagerEditor->isEditorMode() && ImGui::IsWindowFocused()) {
            if (m_pWindow->isMousePressed(MAR_MOUSE_BUTTON_2)) {
                ImGui::OpenPopup("SceneHierarchyPopUp");
            }
        }

        if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
            if (ImGui::MenuItem("Add Entity to scene")) {
                FEventsEntityImGuiWidgets::onCreateEntity();
            }
            if(m_pInspectorWidget->isInspectedEntityValid()) {
                const Entity& entity{ m_pInspectorWidget->getInspectedEntity() };
                const char* tag{ entity.getComponent<TagComponent>().tag.c_str() };
                if(ImGui::MenuItem("Delete Selected Entity from Scene", tag)) {
                    FEventsEntityImGuiWidgets::onDestroyEntity(entity);
                }
            }

            ImGui::EndPopup();
        }
    }


}
