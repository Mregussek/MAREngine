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


#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H


#include "IMeshBatchStatic.h"
#include "MeshDefinitions.h"


namespace marengine {

	class Entity;
	struct RenderableComponent;
	struct TransformComponent;
	struct ColorComponent;


	class FMeshBatchStaticColor : public IMeshBatchStatic {
	public:

		virtual void reset() override;

		virtual bool canBeBatched(const Entity& entity) const override;
		virtual void submitToBatch(const Entity& entity) override;
		
		virtual const FVertexArray& getVertices() const override;
		virtual const FIndicesArray& getIndices() const override;
		virtual const FTransformsArray& getTransforms() const override;
		const FColorsArray& getColors() const;

		uint32_t getUniquePipelineID() const;
		void setUniquePipelineID(uint32_t id);

	protected:

		virtual void submitRenderableComponent(const RenderableComponent& renderableComponent) override;
		virtual void submitVertices(const FVertexArray& vertices) override;
		virtual void submitIndices(const FIndicesArray& indices) override;
		virtual void submitTransformComponent(const TransformComponent& transformComponent) override;
		void submitColorComponent(const ColorComponent& colorComponent);

	private:

		FVertexArray m_vertices;
		FIndicesArray m_indices;
		FTransformsArray m_transforms;
		FColorsArray m_colors;

		float m_meshID{ 0.f };
		uint32_t m_indicesMaxValue{ 0 };
		uint32_t m_uniquePipelineID{ 0 };

	};


}


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
