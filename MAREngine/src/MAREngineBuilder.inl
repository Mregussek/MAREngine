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


#ifndef MARENGINE_MARENGINEBUILDER_INL
#define MARENGINE_MARENGINEBUILDER_INL


#include "MAREngineBuilder.h"


namespace marengine {


    template<typename TWindow, typename TRenderLayer, typename TEditorLayer>
    FLayerStack FMAREngineBuilder<TWindow, TRenderLayer, TEditorLayer>::createLayerStack() const {
        return FLayerStack();
    }

    template<typename TWindow, typename TRenderLayer, typename TEditorLayer>
    IWindow* FMAREngineBuilder<TWindow, TRenderLayer, TEditorLayer>::createWindow() const {
        return (IWindow*)&m_window;
    }

    template<typename TWindow, typename TRenderLayer, typename TEditorLayer>
    FRenderLayer* FMAREngineBuilder<TWindow, TRenderLayer, TEditorLayer>::createRenderLayer() {
        return (FRenderLayer*)&m_renderLayer;
    }

    template<typename TWindow, typename TRenderLayer, typename TEditorLayer>
    FSceneLayer* FMAREngineBuilder<TWindow, TRenderLayer, TEditorLayer>::createSceneLayer() {
        return &m_sceneLayer;
    }

    template<typename TWindow, typename TRenderLayer, typename TEditorLayer>
    FEditorLayer* FMAREngineBuilder<TWindow, TRenderLayer, TEditorLayer>::createEditorLayer() {
        return (FEditorLayer*)&m_editorLayer;
    }


}


#endif //MARENGINE_MARENGINEBUILDER_INL
