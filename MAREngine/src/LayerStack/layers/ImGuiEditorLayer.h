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


#ifndef MARENGINE_IMGUIEDITORLAYER_H
#define MARENGINE_IMGUIEDITORLAYER_H


#include "../Layer.h"
#include "../../Editor/EditorManager.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/ImGuiEditorWidgetsLocator.h"


namespace marengine {

    struct RenderStatistics;
    class FSceneManagerEditor;


    class FImGuiEditorLayer : public Layer {
    public:

        void create(FSceneManagerEditor* pSceneManagerEditor, const RenderStatistics* pRenderStatistic);

        void update() override;
        void close() override;

        void renderToViewport();

    private:

        FEditorManager m_editorManager;
        FImGuiEditorWidgetsLocator m_serviceLocator;
        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };

    };


}


#endif //MARENGINE_IMGUIEDITORLAYER_H