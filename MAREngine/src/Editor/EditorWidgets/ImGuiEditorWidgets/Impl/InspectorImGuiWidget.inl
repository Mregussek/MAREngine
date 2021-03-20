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


#include "InspectorImGuiWidget.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Core/ecs/Components/Components.h"


namespace marengine {


    template<typename TComponent>
    void FInspectorImGuiWidget::handle(const char* componentName) {
        if (m_inspectedEntity->hasComponent<TComponent>() && ImGui::CollapsingHeader(componentName)) {
            displayComponentPanel<TComponent>();
        }
    }

    template<typename TComponent>
    void FInspectorImGuiWidget::displayComponentPanel() {
        ImGui::Text("This component is not yet supported!");
    }


    // Here add declarations for specific component event implementations ...

    template<> void FInspectorImGuiWidget::displayComponentPanel<TagComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<TransformComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<PythonScriptComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<RenderableComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<CameraComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<ColorComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<Texture2DComponent>();
    template<> void FInspectorImGuiWidget::displayComponentPanel<PointLightComponent>();


}


#endif //MARENGINE_INSPECTOREDITORWIDGET_INL
