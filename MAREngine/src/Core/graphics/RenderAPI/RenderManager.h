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


#ifndef MAR_ENGINE_F_RENDER_MANAGER_H
#define MAR_ENGINE_F_RENDER_MANAGER_H


#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"


namespace marengine {

	class RenderCamera;
	class Entity;


	class FRenderManager {

		friend class FRenderManagerEvents;

	public:

		struct SubmitRenderableReturnInfo {
			bool submitted{ false };
			const FMeshBatchStatic* pMeshBatch{ nullptr };
		};


		static FRenderManager* Instance;

		void reset();

		void batchEntities(const std::vector<Entity>& entities);
		SubmitRenderableReturnInfo submitEntityRenderableToBatch(const Entity& entity);
		bool submitEntityLightToBatch(const Entity& entity);
		bool submitEntityCamera(const Entity& entity);

		const FMeshBatchStaticColor& getStaticColorBatch() const;
		const FMeshBatchStaticTexture2D& getStaticTexture2DBatch() const;
		const FPointLightBatch& getPointLightsBatch() const;

		void setRenderCamera(const RenderCamera* renderCamera);

	private:

		bool containsMainCamera(const Entity& entity) const;

		void onBatchesReadyToDraw();

		void createBuffers();
		void fillBuffers();

		uint32_t createVertexIndexBuffers() const;
		uint32_t createTransformsSSBO() const;
		uint32_t createColorSSBO() const;

		uint32_t createPointLightSSBO() const;

		uint32_t createCameraSSBO() const; 


		FMeshBatchStaticColor m_staticColorMeshBatch;
		FMeshBatchStaticTexture2D m_staticTexture2DMeshBatch;
		FPointLightBatch m_pointLightBatch;

		const RenderCamera* m_renderCamera;

	};


}


#endif // !MAR_ENGINE_F_RENDER_MANAGER_H
