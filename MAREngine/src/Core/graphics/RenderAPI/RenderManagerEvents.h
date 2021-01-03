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


#ifndef MAR_ENGINE_F_RENDER_MANAGER_EVENTS_H
#define MAR_ENGINE_F_RENDER_MANAGER_EVENTS_H


namespace marengine {

	class FMeshBatchStatic;
	class FMeshBatchStaticColor;
	class FMeshBatchStaticTexture2D;
	class FPointLightBatch;
	class RenderCamera;
	class Entity;


	class FRenderManagerEvents {
	public:

		static void onVertexIndexBuffersUpdate(const FMeshBatchStatic& meshBatch);
		static void onTransformsUpdate(const FMeshBatchStatic& meshBatch);
		
		static void onColorsUpdate(const FMeshBatchStaticColor& meshColorBatch);

		static void onRenderCameraUpdate(const RenderCamera* renderCamera);

		static void onPointLightUpdate(const FPointLightBatch& pointLightBatch);


		static void onTransformAtMeshUpdate(const Entity& entity);
		static void onColorAtMeshUpdate(const Entity& entity);
		static void onTexture2DatMeshUpdate(const Entity& entity);

		static void onPointLightAtBatchUpdate(const Entity& entity);

	};


}



#endif // !MAR_ENGINE_F_RENDER_MANAGER_EVENTS_H