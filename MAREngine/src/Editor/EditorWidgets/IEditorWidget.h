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


#ifndef MARENGINE_IEDITORWIDGET_H
#define MARENGINE_IEDITORWIDGET_H


#include "../../Core/IWidget.h"


namespace marengine {


    class IEditorWidget : public IWidget {

    };

    class FEditorWidget : public IEditorWidget {

    };


    class IDebugEditorWidget : public FEditorWidget {

    };

    class FDebugEditorWidget : public IDebugEditorWidget {

    };


    class IEnvironmentPropertiesEditorWidget : public FEditorWidget {

    };

    class FEnvironmentPropertiesEditorWidget : public IEnvironmentPropertiesEditorWidget {

    };


    class IInspectorEditorWidget : public FEditorWidget {

    };

    class FInspectorEditorWidget : public IInspectorEditorWidget {

    };


    class IMainEditorWidget : public FEditorWidget {

    };

    class FMainEditorWidget : public IMainEditorWidget {

    };


    class IMainMenuBarEditorWidget : public FEditorWidget {

    };

    class FMainMenuBarEditorWidget : public IMainMenuBarEditorWidget {

    };


    class ISceneHierarchyEditorWidget : public FEditorWidget {

    };

    class FSceneHierarchyEditorWidget : public ISceneHierarchyEditorWidget {

    };


    class IScriptEditorWidget : FEditorWidget {

    };

    class FScriptEditorWidget : public IScriptEditorWidget {

    };


    class IViewportEditorWidget : public FEditorWidget {

    };

    class FViewportEditorWidget : public IViewportEditorWidget {

    };


}


#endif //MARENGINE_IEDITORWIDGET_H

