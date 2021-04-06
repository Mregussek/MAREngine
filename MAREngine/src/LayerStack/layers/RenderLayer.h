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


#ifndef MAR_ENGINE_LAYERS_RENDER_LAYER_H
#define MAR_ENGINE_LAYERS_RENDER_LAYER_H


#include "../../mar.h"
#include "../ILayer.h"
#include "../../Core/graphics/Renderer/Renderer.h"
#include "../../Core/graphics/Renderer/RenderManager.h"
#include "../../Core/graphics/Renderer/OpenGL/GraphicsOpenGL.h"
#include "../../Core/graphics/Renderer/OpenGL/RendererOpenGL.h"


namespace marengine {

    class FWindow;


    class FRenderLayer : public ILayer {
    public:

        virtual void create(FWindow* pWindow) = 0;

        MAR_NO_DISCARD FRenderStatistics* getRenderStats();
		MAR_NO_DISCARD virtual FRenderManager* getRenderManager() = 0;

    protected:

        FRenderStatistics p_statistics;

    };


	class FRenderLayerOpenGL : public FRenderLayer {
    public:

        void create(FWindow* pWindow) final;
        void begin() final;
        void update() final;
        void end() final;
        void close() final;

		MAR_NO_DISCARD FRenderManager* getRenderManager();

	private:

	    FGraphicsContextOpenGL m_context;
		FRenderManager m_manager;
		FRendererOpenGL m_renderer;

	};


}


#endif // !MAR_ENGINE_LAYERS_RENDER_LAYER_H
