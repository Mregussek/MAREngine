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


#ifndef MAR_ENGINE_I_MESH_BATCH_H
#define MAR_ENGINE_I_MESH_BATCH_H


#include "MeshDefinitions.h"


namespace marengine {

	class Entity;
	struct RenderableComponent;
	struct TransformComponent;


	class IMeshBatch {
	public:

		virtual void reset() = 0;

		virtual bool hasAnythingToDraw() const = 0;

		virtual bool canBeBatched(const Entity& entity) const = 0;
		virtual void submitToBatch(const Entity& entity) = 0;

		virtual const FVertexArray& getVertices() const = 0;
		virtual const FIndicesArray& getIndices() const = 0;
		virtual const FTransformsArray& getTransforms() const = 0;

		virtual uint32_t getUniquePipelineID() const = 0;
		virtual void setUniquePipelineID(uint32_t id) = 0;

		virtual uint32_t getTransformsSSBOindex() const = 0;
		virtual void setTransformsSSBOindex(uint32_t index) = 0;

	protected:

		virtual void submitRenderableComponent(const RenderableComponent& renderableComponent) = 0;
		virtual void submitVertices(const FVertexArray& vertices) = 0;
		virtual void submitIndices(const FIndicesArray& indices) = 0;
		virtual void submitTransformComponent(const TransformComponent& transformComponent) = 0;

	};


}



#endif // !MAR_ENGINE_I_MESH_BATCH_H
