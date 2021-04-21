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


#ifndef MARENGINE_EDITORLAYER_H
#define MARENGINE_EDITORLAYER_H


#include "../ILayer.h"
#include "../../Editor/EditorWidgets/EditorWidgetsServiceManager.h"
#include "../../Editor/EditorWidgets/ImGui/ImGuiEditorServiceLocator.h"


namespace marengine {

    class FRenderStatistics;
    class FSceneManagerEditor;
    class FMeshManager;
    class FWindow;
    class FRenderManager;


    class FEditorLayer : public ILayer {
    public:

        virtual void create(FWindow* pWindow,
                            FSceneManagerEditor* pSceneManagerEditor,
                            FMeshManager* pMeshManager,
                            FRenderManager* pRenderManager,
                            FRenderStatistics* pRenderStatistic) = 0;

    };


    class FEditorLayerImGui : public FEditorLayer {
    public:

        void create(FWindow* pWindow,
                    FSceneManagerEditor* pSceneManagerEditor,
                    FMeshManager* pMeshManager,
                    FRenderManager* pRenderManager,
                    FRenderStatistics* pRenderStatistic) final;

        void begin() final;
        void update() final;
        void end() final;
        void close() final;

    private:

        FEditorWidgetsServiceManager m_editorServiceManager;
        FImGuiEditorServiceLocator m_serviceLocator;

        FSceneManagerEditor* m_pSceneManagerEditor{ nullptr };
        FWindow* m_pWindow{ nullptr };

    };


}


#endif //MARENGINE_EDITORLAYER_H
