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


#ifndef MARENGINE_IMGUIEDITORSERVICELOCATOR_H
#define MARENGINE_IMGUIEDITORSERVICELOCATOR_H


#include "../../../mar.h"


namespace marengine {

    class FSceneManagerEditor;
    class FRenderStatistics;
    class FMeshManager;
    class FWindow;
    class FRenderManager;
    class FMaterialManager;
    template<typename TTypeToHold> struct FImGuiTypeHolder;


    class FImGuiEditorServiceLocator {
    public:

        void registerServices(FWindow* pWindow,
                              FSceneManagerEditor* pSceneManagerEditor,
                              FRenderStatistics* pRenderStatistics,
                              FMeshManager* pMeshManager,
                              FRenderManager* pRenderManager,
                              FMaterialManager* pMaterialManager);
        void close();

        template<typename TImGuiService>
        MAR_NO_DISCARD TImGuiService* retrieve();

    private:

        template<typename TImGuiService>
        TImGuiService* emplace();

        entt::registry m_imguiRegistry;
        entt::entity m_imguiEntity{ entt::null };

    };


    template<typename TTypeToHold>
    struct FImGuiTypeHolder {

        TTypeToHold pInstance;

    };


}


#include "ImGuiEditorServiceLocator.inl"


#endif //MARENGINE_IMGUIEDITORSERVICELOCATOR_H
