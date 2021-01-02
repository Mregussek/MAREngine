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


#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_H


#include "IMeshBatch.h"


namespace marengine {


	class FMeshBatchStatic : public IMeshBatch {
	public:

		virtual void reset() override;
		
		virtual bool hasAnythingToDraw() const override;

		virtual bool canBeBatched(const Entity& entity) const override;
		virtual void submitToBatch(const Entity& entity) override;

		virtual const FVertexArray& getVertices() const override;
		virtual const FIndicesArray& getIndices() const override;
		virtual const FTransformsArray& getTransforms() const override;

		virtual uint32_t getUniquePipelineID() const override;
		virtual void setUniquePipelineID(uint32_t id) override;

		virtual uint32_t getTransformsSSBOindex() const override;
		virtual void setTransformsSSBOindex(uint32_t index) override;

	protected:

		virtual void submitRenderableComponent(const RenderableComponent& renderableComponent) override;
		virtual void submitVertices(const FVertexArray& vertices) override;
		virtual void submitIndices(const FIndicesArray& indices) override;
		virtual void submitTransformComponent(const TransformComponent& transformComponent) override;


		FVertexArray p_vertices;
		FIndicesArray p_indices;
		FTransformsArray p_transforms;

		float p_meshID{ 0.f };
		uint32_t p_indicesMaxValue{ 0 };
		uint32_t p_uniquePipelineID{ 0 };
		uint32_t p_transformsSSBOindex{ 0 };

	};


}



#endif // !MAR_ENGINE_I_STATIC_MESH_BATCH_H
