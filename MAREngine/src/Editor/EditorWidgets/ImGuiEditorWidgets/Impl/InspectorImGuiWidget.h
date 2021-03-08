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


#ifndef MARENGINE_INSPECTOREDITORWIDGET_H
#define MARENGINE_INSPECTOREDITORWIDGET_H


#include "../../../../mar.h"
#include "../../IInspectorEditorWidget.h"


namespace marengine {

    class Entity;
    class FSceneManagerEditor;


    class FInspectorImGuiWidget : public IInspectorEditorWidget {
    public:

        void create(FSceneManagerEditor* pSceneManagerEditor);

        void resetInspectedEntity();
        void setEntityToInspect(const Entity& entityToInspect);
        MAR_NO_DISCARD const Entity& getInspectedEntity() const;
        MAR_NO_DISCARD bool isInspectedEntityValid() const;

        void updateFrame() override;

    private:

        void displayPlayMode();
        void displayEditorMode();

        void popUpMenu() const;
        void displayChildrenPopMenu() const;
        void displayComponentPopMenu() const;

        void handleInputs();

        template<typename TComponent>
        void handle(const char* componentName);

        template<typename TComponent>
        void displayComponentPanel();


        const Entity* m_inspectedEntity{ nullptr };
        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };

        bool m_newScriptWindow{ false };
        bool m_assignScriptWindow{ false };

    };


}


#include "InspectorImGuiWidget.inl"


#endif //MARENGINE_INSPECTOREDITORWIDGET_H
