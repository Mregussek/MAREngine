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


#ifndef MARENGINE_INSPECTOREDITORWIDGET_INL
#define MARENGINE_INSPECTOREDITORWIDGET_INL


#include "InspectorImGuiEditorWidgetImpl.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Core/ecs/Components/Components.h"


namespace marengine {


    template<typename TComponent>
    void FInspectorImGuiEditorWidgetImpl::handle(const char* componentName) {
        if (m_inspectedEntity->hasComponent<TComponent>() && ImGui::CollapsingHeader(componentName)) {
            displayComponentPanel<TComponent>();
        }
    }

    template<typename TComponent>
    void FInspectorImGuiEditorWidgetImpl::displayComponentPanel() {
        ImGui::Text("This component is not yet supported!");
    }


    // Here add declarations for specific component event implementations ...

    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<TagComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<TransformComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<PythonScriptComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<RenderableComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<CameraComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<ColorComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<Texture2DComponent>();
    template<> void FInspectorImGuiEditorWidgetImpl::displayComponentPanel<PointLightComponent>();


}


#endif //MARENGINE_INSPECTOREDITORWIDGET_INL
