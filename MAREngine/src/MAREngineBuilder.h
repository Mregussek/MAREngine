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


#ifndef MARENGINE_MARENGINEBUILDER_H
#define MARENGINE_MARENGINEBUILDER_H


#include "LayerStack/LayerStack.h"
#include "Window/GLFW/WindowGLFW.h"
#include "Window/SDL/WindowSDL.h"
#include "LayerStack/layers/EditorLayer.h"
#include "LayerStack/layers/SceneLayer.h"
#include "LayerStack/layers/RenderLayer.h"


namespace marengine {


    class IMAREngineBuilder {
    public:

        MAR_NO_DISCARD virtual FLayerStack createLayerStack() const = 0;

        MAR_NO_DISCARD virtual FWindow* createWindow() const = 0;

        MAR_NO_DISCARD virtual FRenderLayer* createRenderLayer() = 0;
        MAR_NO_DISCARD virtual FSceneLayer* createSceneLayer() = 0;
        MAR_NO_DISCARD virtual FEditorLayer* createEditorLayer() = 0;

    };


    template<typename TWindow, typename TRenderLayer, typename TEditorLayer>
    class FMAREngineBuilder : public IMAREngineBuilder {
    public:

        MAR_NO_DISCARD FLayerStack createLayerStack() const final;
        MAR_NO_DISCARD FWindow* createWindow() const final;
        MAR_NO_DISCARD FRenderLayer* createRenderLayer() final;
        MAR_NO_DISCARD FSceneLayer* createSceneLayer() final;
        MAR_NO_DISCARD FEditorLayer* createEditorLayer() final;

    private:

        TWindow m_window;
        TEditorLayer m_editorLayer;
        TRenderLayer m_renderLayer;

        FSceneLayer m_sceneLayer;

    };


}


#include "MAREngineBuilder.inl"


#endif //MARENGINE_MARENGINEBUILDER_H
