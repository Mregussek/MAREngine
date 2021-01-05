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


#ifndef MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H
#define MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H


#include "../../../mar.h"
#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"
#include "../../ecs/Components/Components.h"


namespace marengine {

	class RenderCamera;
	class Entity; 


	class RenderPipeline {

		friend class RenderEvents;

	public:

		RenderPipeline() = default;

		void reset();

		void pushEntityToPipeline(const Entity& entity);
		void pushCameraToPipeline(const RenderCamera* cam);

		static RenderPipeline* Instance;

		// ---- GETTERS for container ---- //
	
		const std::vector<FMeshBatchStaticColor>& getColorBatches() const;
		const std::vector<FMeshBatchStaticTexture2D>& getTexture2DBatches() const;
		const std::vector<FPointLightBatch>& getPointLightBatches() const;
		const RenderCamera* getCamera() const;

	private:

		uint32_t getAvailableColorBatch(const Entity& entity);
		uint32_t getAvailableTexture2DBatch(const Entity& entity);
		uint32_t getAvailablePointLightBatch(const Entity& entity);

		std::vector<FMeshBatchStaticColor> m_staticColorBatches;
		std::vector<FMeshBatchStaticTexture2D> m_staticTexture2DBatches;
		std::vector<FPointLightBatch> m_pointLightBatches;

		const RenderCamera* m_camera{ nullptr };

	};


}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_PIPELINE_H

