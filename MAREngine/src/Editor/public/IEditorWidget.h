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
#include "../../mar.h"


namespace marengine {

    class FServiceLocatorEditor;
    struct CRenderable;
    class Entity;

    enum class EEditorContextType {
        NONE, IMGUI
    };


    class IEditorWidget : public IWidget {
    public:

        virtual void create(FServiceLocatorEditor* pServiceLocator) = 0;

    };

    class FEditorWidget : public IEditorWidget {

    };


    class IDebugEditorWidget : public FEditorWidget {

    };


    class IEnvironmentPropertiesEditorWidget : public FEditorWidget {

    };


    class IInspectorEditorWidget : public FEditorWidget {
    public:

        virtual void resetInspectedEntity() = 0;
        virtual void setInspectedEntity(const Entity& entity) = 0;
        virtual const Entity& getInspectedEntity() const = 0;
        virtual bool isInspectedEntityValid() const = 0;

    };


    class IMainEditorWidget : public FEditorWidget {

    };


    class IMainMenuBarEditorWidget : public FEditorWidget {

    };


    class ISceneHierarchyEditorWidget : public FEditorWidget {

    };


    class IScriptEditorWidget : public FEditorWidget {
    public:

        virtual void reset() = 0;

        virtual void setEditorTitle(std::string newTitle) = 0;
        virtual void setEditorCode(const std::string& sourceCode) = 0;
        virtual void setPathToScript(std::string pathToScript) = 0;

        virtual std::string getEditorSourceCode() const = 0;
        virtual std::string getDefaultEditorSourceCode() const = 0;
        virtual std::string getDefaultEditorTitle() const = 0;

        virtual bool isEditorCurrentlyUsed() const = 0;

    };


    class IViewportEditorWidget : public FEditorWidget {

    };


    class IContentBrowserEditorWidget : public FEditorWidget {
    public:

        virtual bool drawMeshListBox(CRenderable& cRenderable) const = 0;

    };


}


#endif //MARENGINE_IEDITORWIDGET_H

