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


#ifndef MAR_ENGINE_F_RENDER_BUFFER_MANAGER_H
#define MAR_ENGINE_F_RENDER_BUFFER_MANAGER_H


#include "../../../mar.h"


namespace marengine {

	class FMeshBatchStaticColor;
	class FMeshBatchStaticTexture2D;
	class FPointLightBatch;
	class RenderCamera;


	class FRenderBufferManager {
	public:

		static void onMeshBatchReadyToDraw(std::vector<FMeshBatchStaticColor>& colorBatches);
		static void onMeshBatchReadyToDraw(std::vector<FMeshBatchStaticTexture2D>& textureBatches);
		static void onLightBatchReadyToDraw(std::vector<FPointLightBatch>& lightBatches);
		static void onRenderCameraReadyToDraw(const RenderCamera* renderCamera);


		static void onPipelineUpdate(const FMeshBatchStaticColor& batch);
		static void onTransformsUpdate(const FMeshBatchStaticColor& batch);
		static void onColorUpdate(const FMeshBatchStaticColor& batch);

		static void onPipelineUpdate(const FMeshBatchStaticTexture2D& batch);
		static void onTransformsUpdate(const FMeshBatchStaticTexture2D& batch);

		static void onLightUpdate(const FPointLightBatch& light);

		static void onRenderCameraUpdate(const RenderCamera* renderCamera);

	};


}


#endif // !MAR_ENGINE_F_RENDER_BUFFER_MANAGER_H
