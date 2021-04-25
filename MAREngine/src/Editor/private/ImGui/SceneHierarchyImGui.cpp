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


#include "SceneHierarchyImGui.h"
#include "InspectorImGui.h"
#include "../../public/ServiceLocatorEditor.h"
#include "../../public/EventsEntityEditor.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Window/Window.h"


namespace marengine {

    static void treesFor(const FEntityArray& entities);


    void FSceneHierarchyWidgetImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pSceneManagerEditor =
                pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        m_pWindow =
                pServiceLocator->retrieve<FHolderPtr<FWindow*>>()->pInstance;
        m_pInspectorWidget =
                pServiceLocator->retrieve<FInspectorWidgetImGui>();
    }

    void FSceneHierarchyWidgetImGui::updateFrame() {
        ImGui::Begin("Scene Hierarchy");

        buttonsAtPanel();

        ImGui::Text("SCENE - %s", m_pSceneManagerEditor->getScene()->getName().c_str());
        ImGui::Separator();

        const FEntityArray& entities{ m_pSceneManagerEditor->getScene()->getEntities() };
        treesFor(entities);

        popUpMenu();

        ImGui::End();
    }

    void treesFor(const FEntityArray& entities) {
        auto displayTreeOrMenuItemRecursively = [](const Entity& entity) {
            constexpr ImGuiTreeNodeFlags treeNodeFlags{
                    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth
            };
            const char* entityTag{ entity.getComponent<CTag>().tag.c_str() };

            if (entity.hasChildren()) { // if entity has children we want tree nodes
                const bool isTreeOpen{ ImGui::TreeNodeEx(entityTag, treeNodeFlags) };
                if (ImGui::IsItemClicked()) {
                    FEventsEntityEditor::onSelectedEntity(entity);

                }
                if (isTreeOpen) {
                    const FEntityArray& children{ entity.getChildren() };
                    treesFor(children);

                    ImGui::TreePop();
                }
            }
            else { // if not normal menu item
                if (ImGui::MenuItem(entityTag)) {
                    FEventsEntityEditor::onSelectedEntity(entity);
                }
            }
        };

        std::for_each(entities.cbegin(), entities.cend(), displayTreeOrMenuItemRecursively);
    }

    void FSceneHierarchyWidgetImGui::buttonsAtPanel() const {
        if (ImGui::Button("+ E")) {
            FEventsEntityEditor::onCreateEntity();
        }

        ImGui::SameLine();

        if(m_pInspectorWidget->isInspectedEntityValid()) {
            ImGui::SameLine();

            if(ImGui::Button("Copy - E")) {
                FEventsEntityEditor::onCopyEntity(m_pInspectorWidget->getInspectedEntity());
            }

            ImGui::SameLine();

            if(ImGui::Button("- E")) {
                FEventsEntityEditor::onDestroyEntity(m_pInspectorWidget->getInspectedEntity());
            }
        }
    }

    void FSceneHierarchyWidgetImGui::popUpMenu() const {
        if (m_pSceneManagerEditor->isEditorMode() && ImGui::IsWindowFocused()) {
            if (m_pWindow->isMousePressed(MAR_MOUSE_BUTTON_2)) {
                ImGui::OpenPopup("SceneHierarchyPopUp");
            }
        }

        if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
            if (ImGui::MenuItem("Add Entity to scene")) {
                FEventsEntityEditor::onCreateEntity();
            }
            if(m_pInspectorWidget->isInspectedEntityValid()) {
                const Entity& entity{ m_pInspectorWidget->getInspectedEntity() };
                const char* tag{ entity.getComponent<CTag>().tag.c_str() };
                if(ImGui::MenuItem("Delete Selected Entity from Scene", tag)) {
                    FEventsEntityEditor::onDestroyEntity(entity);
                }
            }

            ImGui::EndPopup();
        }
    }


}
