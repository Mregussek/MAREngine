/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "RenderLayer.h"
#include "../../Window/Window.h"


namespace marengine {


	RenderLayer::RenderLayer(const char* name) {
		p_debugName = name;
	}

	void RenderLayer::initialize() {
		LAYER_TRACE("RENDER_LAYER: {} going to initialize...", p_debugName);

		m_memorizer.Instance = &m_memorizer;
		m_statistics.Instance = &m_statistics;
		m_shaderBufferStorage.Instance = &m_shaderBufferStorage;
		m_pipelineStorage.Instance = &m_pipelineStorage;
		m_renderManager.Instance = &m_renderManager;

		m_renderer.initialize();
	}

	void RenderLayer::update() {
		LAYER_TRACE("RENDER_LAYER: {} going to update...", p_debugName);

		m_statistics.reset();

		const FPointLightBatch& batchPointLight{ m_renderManager.getPointLightsBatch() };

		const FMeshBatchStaticColor& batchStaticColor{ m_renderManager.getStaticColorBatch() };
		m_renderer.draw(batchStaticColor, batchPointLight);

		const FMeshBatchStaticTexture2D& batchStaticTexture2D{ m_renderManager.getStaticTexture2DBatch() };
		m_renderer.draw(batchStaticTexture2D, batchPointLight);

		m_statistics.update();
	}

	void RenderLayer::close() {
		LAYER_TRACE("RENDER_LAYER: {} going to close...", p_debugName);
	
		m_shaderBufferStorage.close();
		m_pipelineStorage.close();

		m_renderer.close();
	}


}
