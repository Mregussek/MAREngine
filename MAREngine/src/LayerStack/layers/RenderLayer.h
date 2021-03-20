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
#include "../../Core/graphics/RenderAPI/RenderPipeline.h"
#include "../../Core/graphics/RenderAPI/RenderStatistics.h"
#include "../../Core/graphics/Renderer/Renderer.h"
#include "../../Core/graphics/Renderer/RenderMemorizer.h"
#include "../../Core/graphics/Renderer/PipelineManager.h"
#include "../../Core/graphics/RenderAPI/OpenGL/RenderApiContextOpenGL.h"


namespace marengine {


    class FRenderLayer : public ILayer {
    public:

        virtual void create() = 0;

        MAR_NO_DISCARD RenderStatistics* getRenderStats();

    protected:

        RenderStatistics p_statistics;

    };


	class FRenderLayerOpenGL : public FRenderLayer {
	public:

	    void create() final;
		void begin() final;
	    void update() final;
	    void end() final;
		void close() final;

	private:

		FRenderer m_renderer;
		RenderPipeline m_renderPipeline;
		FPipelineManager m_pipelineManager;
		RenderMemorizer m_memorizer;
        FRenderApiContextOpenGL m_renderApiContext;

	};


}


#endif // !MAR_ENGINE_LAYERS_RENDER_LAYER_H
