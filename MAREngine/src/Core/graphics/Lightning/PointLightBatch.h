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


<<<<<<< HEAD
<<<<<<< HEAD:MAREngine/src/Core/graphics/Lightning/PointLightBatch.h
=======
>>>>>>> renderissue
#ifndef MAR_ENGINE_F_POINT_LIGHT_BATCH_H
#define MAR_ENGINE_F_POINT_LIGHT_BATCH_H


#include "LightDefinitions.h"
<<<<<<< HEAD
=======
#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_TEXTURE2D_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_TEXTURE2D_H


#include "MeshBatchStatic.h"
#include "MeshDefinitions.h"
>>>>>>> renderissue:MAREngine/src/Core/graphics/Mesh/MeshBatchStaticTexture2D.h
=======
>>>>>>> renderissue


namespace marengine {

	class Entity;
<<<<<<< HEAD
<<<<<<< HEAD:MAREngine/src/Core/graphics/Lightning/PointLightBatch.h
=======
	struct Texture2DComponent;
>>>>>>> renderissue:MAREngine/src/Core/graphics/Mesh/MeshBatchStaticTexture2D.h

	
	class FMeshBatchStaticTexture2D : public FMeshBatchStatic {

		friend class FEventsMeshBatchStatic;

<<<<<<< HEAD:MAREngine/src/Core/graphics/Lightning/PointLightBatch.h
	class FPointLightBatch {

		friend class FRenderManagerEvents;
=======


	class FPointLightBatch {

		friend class FEventsLightBatch;
>>>>>>> renderissue

	public:

		void reset();

		bool hasAnythingToDraw() const;

		bool canBeBatched(const Entity& entityWithLight) const;
		void submitEntityWithLightning(const Entity& entity);

		const FPointLightsArray& getLights() const;

<<<<<<< HEAD
		uint32_t getPointLightSSBOindex() const;
		void setPointLightSSBOindex(uint32_t index);
=======
		uint32_t getUniquePointLightID() const;
		void setUniquePointLightID(uint32_t index);
>>>>>>> renderissue

	private:

		FPointLightsArray m_lights;
<<<<<<< HEAD
		uint32_t m_pointLightSSBOindex{ 0 };
=======
	public:

		virtual void reset() override;

		virtual bool canBeBatched(const Entity& entity) const override;

		virtual void submitToBatch(const Entity& entity) override;

		const FTexturesArray& getTextures() const;

	private:

		void submitTexture(uint32_t bindingIndex, const Texture2DComponent& textureComponent);


		FTexturesArray m_textures;
		static const EMeshBatchStaticType s_meshBatchType{ EMeshBatchStaticType::TEXTURE2D };
>>>>>>> renderissue:MAREngine/src/Core/graphics/Mesh/MeshBatchStaticTexture2D.h
=======
		uint32_t m_uniquePointLightID{ 0 };
>>>>>>> renderissue

	};
	

}


<<<<<<< HEAD
<<<<<<< HEAD:MAREngine/src/Core/graphics/Lightning/PointLightBatch.h

#endif // !MAR_ENGINE_F_POINT_LIGHT_BATCH_H
=======
#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_TEXTURE2D_H
>>>>>>> renderissue:MAREngine/src/Core/graphics/Mesh/MeshBatchStaticTexture2D.h
=======

#endif // !MAR_ENGINE_F_POINT_LIGHT_BATCH_H
>>>>>>> renderissue
