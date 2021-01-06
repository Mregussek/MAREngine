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


#ifndef MAR_ENGINE_LAYERS_RENDER_LAYER_H
#define MAR_ENGINE_LAYERS_RENDER_LAYER_H


#include "../../mar.h"
#include "../Layer.h"
#include "../../Core/graphics/RenderAPI/RenderStatistics.h"
#include "../../Core/graphics/RenderAPI/RenderManager.h"
#include "../../Core/graphics/Renderer/RendererBatch.h"
#include "../../Core/graphics/Renderer/RenderMemorizer.h"
#include "../../Core/graphics/Renderer/PipelineManager.h"


namespace marengine {


	class RenderLayer : public Layer {
	public:

		RenderLayer() = default;
		RenderLayer(const char* name);

		// --- OVERRIDED METHODS --- //

		void initialize() override;
		void update() override;
		void close() override;

	private:

		RendererBatch m_renderer;
		RenderMemorizer m_memorizer;
		PipelineStorage m_pipelineStorage;
		FRenderManager m_renderManager;

	};


}


#endif // !MAR_ENGINE_LAYERS_RENDER_LAYER_H
