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


#ifndef MARENGINE_SCENEHIERARCHYIMGUIWIDGET_H
#define MARENGINE_SCENEHIERARCHYIMGUIWIDGET_H


#include "../../ISceneHierarchyEditorWidget.h"
#include "../../../../Core/ecs/Entity/EntityDefinitions.h"


namespace marengine {

    class FSceneManagerEditor;
    class FInspectorImGuiWidget;


    class FSceneHierarchyImGuiWidget : public ISceneHierarchyEditorWidget {
    public:

        void create(FSceneManagerEditor* pSceneManagerEditor, FInspectorImGuiWidget* pInspectorWidget);
        void updateFrame() override;

    private:

        void treesFor(const FEntityArray& entities) const;
        void buttonsAtPanel() const;
        void popUpMenu() const;


        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FInspectorImGuiWidget* m_pInspectorWidget{ nullptr };

    };


}



#endif //MARENGINE_SCENEHIERARCHYIMGUIWIDGET_H
