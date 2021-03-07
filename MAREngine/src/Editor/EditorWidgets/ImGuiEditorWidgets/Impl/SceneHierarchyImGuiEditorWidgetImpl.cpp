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


#include "SceneHierarchyImGuiEditorWidgetImpl.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Window/Window.h"


namespace marengine {


    void FSceneHierarchyImGuiEditorWidgetImpl::create(FSceneManagerEditor *pSceneManagerEditor) {
        m_pSceneManagerEditor = pSceneManagerEditor;
    }

    void FSceneHierarchyImGuiEditorWidgetImpl::updateFrame() {
        ImGui::Begin("Scene Hierarchy");

        buttonsAtPanel();

        ImGui::Text("SCENE - %s", m_pSceneManagerEditor->getScene()->getName().c_str());
        ImGui::Separator();

        const FEntityArray& entities{ m_pSceneManagerEditor->getScene()->getEntities() };
        treesFor(entities);

        popUpMenu();

        ImGui::End();
    }

    void FSceneHierarchyImGuiEditorWidgetImpl::treesFor(const FEntityArray& entities) const {
        auto displayTreeOrMenuItemRecursively = [this](const Entity& entity) {
            constexpr ImGuiTreeNodeFlags treeNodeFlags{
                    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth
            };
            const char* entityTag{ entity.getComponent<TagComponent>().tag.c_str() };

            if (entity.hasChilds()) { // if entity has children we want tree nodes
                const bool isTreeOpen{ ImGui::TreeNodeEx(entityTag, treeNodeFlags) };
                if (ImGui::IsItemClicked()) {
                    // TODO: add selected entity option on scene hierarchy panel
                    //FEventsEntityWidget::Instance->onSelectedEntity(entity);
                }
                if (isTreeOpen) {
                    const FEntityArray& children{ entity.getChilds() };
                    treesFor(children);

                    ImGui::TreePop();
                }
            }
            else { // if not normal menu item
                if (ImGui::MenuItem(entityTag)) {
                    // TODO: add selected entity option on scene hierarchy panel
                    //FEventsEntityWidget::Instance->onSelectedEntity(entity);
                }
            }
        };

        std::for_each(entities.cbegin(), entities.cend(), displayTreeOrMenuItemRecursively);
    }

    void FSceneHierarchyImGuiEditorWidgetImpl::buttonsAtPanel() const {
        if (ImGui::Button("+ E")) {
            // TODO: add create entity option on scene hierarchy panel
            //FEventsEntityWidget::Instance->onCreateEntity();
        }

        ImGui::SameLine();

        // TODO: Add reference to entity inspector panel for currently inspecting entity
        //const auto& entity = WEntityWidgetPanel::Instance->getCurrentEntity();
        //const bool entityExists = &entity != nullptr;

        //if (entityExists) {
        //    ImGui::SameLine();

        //    if (ImGui::Button("Copy - E")) {
        //        FEventsEntityWidget::Instance->onCopyEntity(entity);
        //    }

        //    ImGui::SameLine();

        //    if (ImGui::Button("- E")) {
        //        FEventsEntityWidget::Instance->onDestroyEntity(entity);
        //    }
        //}
    }

    void FSceneHierarchyImGuiEditorWidgetImpl::popUpMenu() const {
        if (m_pSceneManagerEditor->isEditorMode() && ImGui::IsWindowFocused()) {
            if (Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
                ImGui::OpenPopup("SceneHierarchyPopUp");
            }
        }

        if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
            if (ImGui::MenuItem("Add Entity to scene")) {
                // TODO: add back entity create event
                //FEventsEntityWidget::Instance->onCreateEntity();
            }

            // TODO: add back delete entity from scene event
            //const Entity& entity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
            //const bool entityExists = &entity != nullptr;

            //if (entityExists) {
            //    const char* entityTag{ entity.getComponent<TagComponent>().tag.c_str() };
            //    if (ImGui::MenuItem("Delete Selected Entity from Scene", entityTag)) {
            //        FEventsEntityWidget::Instance->onDestroyEntity(entity);
            //    }

            //}

            ImGui::EndPopup();
        }
    }


}
