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


#ifndef MARENGINE_EDITORWIDGET_H
#define MARENGINE_EDITORWIDGET_H


#include "IEditorWidget.h"


namespace marengine {

    class Entity;


    class FDebugEditorWidget : public IDebugEditorWidget {

    };


    class FEnvironmentPropertiesEditorWidget : public IEnvironmentPropertiesEditorWidget {

    };


    class FInspectorEditorWidget : public IInspectorEditorWidget {
    public:

        void resetInspectedEntity() final;
        void setInspectedEntity(const Entity& entity) final;
        MAR_NO_DISCARD const Entity& getInspectedEntity() const final;
        MAR_NO_DISCARD bool isInspectedEntityValid() const final;

    protected:

        const Entity* p_pInspectedEntity{ nullptr };

    };


    class FMainEditorWidget : public IMainEditorWidget {

    };


    class FMainMenuBarEditorWidget : public IMainMenuBarEditorWidget {

    };


    class FSceneHierarchyEditorWidget : public ISceneHierarchyEditorWidget {

    };


    class FScriptEditorWidget : public IScriptEditorWidget {

    };


    class FViewportEditorWidget : public IViewportEditorWidget {

    };


    class FContentBrowserEditorWidget : public IContentBrowserEditorWidget {

    };


}


#endif //MARENGINE_EDITORWIDGET_H
