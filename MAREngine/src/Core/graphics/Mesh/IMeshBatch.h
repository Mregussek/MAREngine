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


#ifndef MAR_ENGINE_I_MESH_BATCH_H
#define MAR_ENGINE_I_MESH_BATCH_H


#include "../Renderer/IRender.h"


namespace marengine {

	class Entity;
	struct RenderableComponent;
	struct TransformComponent;


	/**
	 * @class IMeshBatch IMeshBatch.h "Core/graphics/Mesh/IMeshBatch.h"
	 * @brief Abstract base class for all mesh batches. Used for optimized
	 * rendering procedure. All derived classes should use those method!
	 */
	class IMeshBatch {
	public:

		/// @brief Method should reset the whole batch class, clear all buffers and so on.
		virtual void reset() = 0;

		/**
		 * @brief Method should return all batched vertices, that can be rendered
		 * @warning To render we need also FIndicesArray and FTransformArray!
		 * @return FVertexArray object ready to render
		 */
		virtual const FVertexArray& getVertices() const = 0;

		/**
		 * @brief Method should return all batched indices, that can be rendered
		 * @warning To render we need also FVerticesArray and FTransformArray!
		 * @return FIndicesArray object ready to render
		 */
		virtual const FIndicesArray& getIndices() const = 0;

		/**
		 * @brief Method should return all transform matrices batched together
		 * @warning To render we need also FVerticesArray and FIndicesArray !
		 * @return FTransformsArray object ready to render
		 */
		virtual const FTransformsArray& getTransforms() const = 0;

		/**
		 * @brief Method should check, if given entity can be batched in current derived MeshBatch instance.
		 * Afterwards result of check should be returned
		 * @attention Entity can be rendered only then, if it has assigned RenderableComponent and some TextureComponent.
		 * Then method must check, if there is place in batch (if vertices / indices will overflow, if shader can render this
		 * properly and so on).
		 * @param entity given entity instance, that we want to be rendered
		 * @return Returns true, if entity can be rendered properly with this batch.
		 */
		virtual bool canBeBatched(const Entity& entity) const = 0;

		/**
		 * @brief Method is responsible for submitting entity to batch. Make sure to check if entity
		 * can be submitted with canBeBatched(entity) method. 
		 * @warning Method does not check, if entity can be batched! Please do this before calling this method.
		 * @param entity entity instance, that can be batched and we want it to be submitted to this batch.
		 */
		virtual void submitToBatch(const Entity& entity) = 0;

		/**
		 * @brief Sets unique pipeline ID. We don't want to retrieve prepared pipeline for this batch,
		 * we want the batch to remember it. So that batch, before render, can just bind needed pipeline buffers.
		 * So, during pipeline creation process call this method to assign unique pipeline id (which should be then
		 * properly interpreted by FPipelineManager).
		 * @param id assigned pipeline id
		 */
		virtual void setUniquePipelineID(uint32_t id) = 0;

		/**
		 * @brief Method should return previously assigned unique pipeline id.
		 * @return Assigned pipeline id.
		 */
		virtual uint32_t getUniquePipelineID() const = 0;

		/**
		 * @brief Sets unique transform ID. We don't want to retrieve prepared transforms SSBO for this batch,
		 * we want the batch to remember it. So that batch, before render, can just bind needed transform SSBO.
		 * So, during transform SSBO creation process call this method to assign unique pipeline id (which should be
		 * then properly interpreted by FPipelineManager).
		 * @param id assigned transforms ID
		 */
		virtual void seUniqueTransformsID(uint32_t id) = 0;

		/**
		 * @brief Method should return previously assigned unique transform id.
		 * @return Assigned transform id.
		 */
		virtual uint32_t getUniqueTransformsID() const = 0 ;

		/**
		 * @brief Returns batch type
		 * @return
		 */
        virtual EBatchType getBatchType() const = 0;

	protected:

		virtual void submitRenderable(const RenderableComponent& renderableComponent) = 0;
		virtual void submitVertices(const FVertexArray& vertices) = 0;
		virtual void submitIndices(const FIndicesArray& indices) = 0;
		
		virtual void submitTransform(const TransformComponent& transformComponent) = 0;

	};
	

}


#endif // !MAR_ENGINE_I_MESH_BATCH_H
