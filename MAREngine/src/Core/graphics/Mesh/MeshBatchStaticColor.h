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


#include "MeshDefinitions.h"


namespace marengine {

	class Entity;
	struct RenderableComponent;
	struct TransformComponent;
	struct ColorComponent;


	class FMeshBatchStaticColor {

		friend class RenderEvents;

	public:

		void reset();

		bool canBeBatched(const Entity& entity) const;

		void submitToBatch(const Entity& entity);

		const FVertexArray& getVertices() const;
		const FIndicesArray& getIndices() const;
		const FTransformsArray& getTransforms() const;
		const FColorsArray& getColors() const;

		uint32_t getUniquePipelineID() const;
		void setUniquePipelineID(uint32_t id);

	private:

		void submitRenderable(const RenderableComponent& renderableComponent);
		void submitVertices(const FVertexArray& vertices);
		void submitIndices(const FIndicesArray& indices);
		void submitTransform(const TransformComponent& transformComponent);
		void submitColor(const ColorComponent& colorComponent);


		FVertexArray m_vertices;
		FIndicesArray m_indices;
		FTransformsArray m_transforms;
		FColorsArray m_colors;

		float m_shapeID{ 0.f };
		uint32_t m_indicesMaxValue{ 0 };
		uint32_t m_uniquePipelineID{ 0 };

	};



}


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
