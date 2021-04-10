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


#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_H


#include "IMeshBatch.h"
#include "../IRender.h"


namespace marengine {

	class Entity;
	struct RenderableComponent;
	struct TransformComponent;
	struct ColorComponent;
	struct Texture2DComponent;


	class FMeshBatchStatic : public IMeshBatch {

		friend class FEventsMeshBatchStatic;

	public:

		void reset() override;

		MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;

		void submitToBatch(const Entity& entity) override;

		MAR_NO_DISCARD const FVertexArray& getVertices() const final;
		MAR_NO_DISCARD const FIndicesArray& getIndices() const final;
		MAR_NO_DISCARD const FTransformsArray& getTransforms() const final;

        MAR_NO_DISCARD uint32_t getUniquePipelineID() const override;
		void setUniquePipelineID(uint32_t id) override;

        MAR_NO_DISCARD uint32_t getUniqueTransformsID() const override;
		void seUniqueTransformsID(uint32_t id) override;

	protected:

		void submitRenderable(const RenderableComponent& renderableComponent) final;
		void submitVertices(const FVertexArray& vertices) final;
		void submitIndices(const FIndicesArray& indices) final;
		void submitTransform(const TransformComponent& transformComponent) final;


		FVertexArray p_vertices;
		FIndicesArray p_indices;
		FTransformsArray p_transforms;

		float p_shapeID{ 0.f };
		uint32_t p_indicesMaxValue{ 0 };
		uint32_t p_uniquePipelineID{ 0 };
		uint32_t p_transformsUniqueID{ 0 };

    };


	class FMeshBatchStaticColor : public FMeshBatchStatic {

		friend class FEventsMeshBatchStatic;

	public:

		void reset() override;

        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;

		void submitToBatch(const Entity& entity) override;

        MAR_NO_DISCARD const FColorsArray& getColors() const;

        MAR_NO_DISCARD uint32_t getUniqueColorsID() const;

		void setUniqueColorsID(uint32_t id);

		MAR_NO_DISCARD EBatchType getBatchType() const override;

	private:

		void submitColor(const ColorComponent& colorComponent);


		FColorsArray m_colors;
		uint32_t m_uniqueColorsID{ 0 };
	
	};

    typedef std::vector<FMeshBatchStaticColor> FMeshBatchStaticColorArray;


	class FMeshBatchStaticTexture2D : public FMeshBatchStatic {
	public:

		void reset() override;

        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;

		void submitToBatch(const Entity& entity) override;

        MAR_NO_DISCARD const FTexturesArray& getTextures() const;

		MAR_NO_DISCARD EBatchType getBatchType() const override;

	private:

		void submitTexture(uint32_t bindingIndex, const Texture2DComponent& textureComponent);


		FTexturesArray m_textures;

	};

    typedef std::vector<FMeshBatchStaticTexture2D> FMeshBatchStaticTexture2DArray;


}


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_H
