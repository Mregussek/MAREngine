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


#include "RenderLayer.h"


namespace marengine {


    RenderStatistics* FRenderLayer::getRenderStats() {
        return &p_statistics;
    }


	void FRenderLayerOpenGL::create(FWindow* pWindow) {
        const bool isRenderApiCreated = m_renderApiContext.create();
        if(!isRenderApiCreated) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Render API!");
            return;
        }

		RenderMemorizer::Instance = &m_memorizer;
		RenderStatistics::Instance = &p_statistics;
		RenderPipeline::Instance = &m_renderPipeline;
		FPipelineManager::Instance = &m_pipelineManager;
		
		m_renderer.initialize();
	}

	void FRenderLayerOpenGL::begin() {

    }

	void FRenderLayerOpenGL::update() {
        p_statistics.reset();
		m_renderer.draw();
	}

	void FRenderLayerOpenGL::end() {

    }

	void FRenderLayerOpenGL::close() {
		m_renderPipeline.reset();
		m_pipelineManager.close();

		m_renderer.close();
	}


    void FRenderLayerOpenGL2::create(FWindow* pWindow) {
        m_context.create(pWindow);
        m_manager.create((FGraphicsContext*)&m_context);
    }

    void FRenderLayerOpenGL2::begin() {
        m_context.prepareFrame();
    }

    void FRenderLayerOpenGL2::update() {
        p_statistics.reset();

        const auto& pipelines{ m_manager.getPipelines() };
        for(const auto& pipeline : pipelines) {
            m_renderer.draw(m_context.getFactory(), &pipeline);
        }
    }

    void FRenderLayerOpenGL2::end() {

    }

    void FRenderLayerOpenGL2::close() {

    }

    FRenderManager* FRenderLayerOpenGL2::getRenderManager() {
        return &m_manager;
    }


}
