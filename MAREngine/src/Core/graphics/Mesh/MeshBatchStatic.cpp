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


#include "MeshBatchStatic.h"
#include "../GraphicsLimits.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	void FMeshBatchStatic::reset() {
		p_vertices.clear();
		p_indices.clear();
		p_transforms.clear();

		p_shapeID = 0.f;
		p_indicesMaxValue = 0;
		p_uniquePipelineID = 0;
	}

	bool FMeshBatchStatic::canBeBatched(const Entity& entity) const {
		const bool entityDoesntHaveRenderable{ !entity.hasComponent<RenderableComponent>() };
		if (entityDoesntHaveRenderable) {
			return false;
		}

		const auto& renderableComponent{ entity.getComponent<RenderableComponent>() };
		const size_t verticesToPush{ renderableComponent.vertices.size() };
		const size_t indicesToPush{ renderableComponent.indices.size() };

		const size_t currentVerticesSize{ p_vertices.size() };
		const size_t currentIndicesSize{ p_indices.size() };
		const size_t currentTransformSize{ p_transforms.size() };

		const bool cannotPushVertices = (currentVerticesSize + verticesToPush) >= GraphicLimits::maxVerticesCount;
		const bool cannotPushIndices = (currentIndicesSize + indicesToPush) >= GraphicLimits::maxIndicesCount;
		const bool cannotPushTransform = (currentTransformSize + 1) >= GraphicLimits::maxTransforms;

		const bool placeInBatchExist{ !(cannotPushVertices || cannotPushIndices || cannotPushTransform) };

		return placeInBatchExist; // true if there is placed
	}

	void FMeshBatchStatic::submitToBatch(const Entity& entity) {
		submitRenderable(entity.getComponent<RenderableComponent>());
		submitTransform(entity.getComponent<TransformComponent>());
	}

	void FMeshBatchStatic::submitRenderable(const RenderableComponent& renderableComponent) {
		submitVertices(renderableComponent.vertices);
		submitIndices(renderableComponent.indices);

		p_indicesMaxValue += (renderableComponent.vertices.size() * sizeof(Vertex) / 4) / g_MeshStride;
		p_shapeID++;
	}

	void FMeshBatchStatic::submitVertices(const FVertexArray& vertices) {
		p_vertices.insert(p_vertices.end(), vertices.begin(), vertices.end());

		auto fromBeginOfInsertedVertices = p_vertices.end() - vertices.size();
		auto toItsEnd = p_vertices.end();
		auto modifyShaderID = [this](Vertex& vertex) {
			vertex.shapeID = p_shapeID;
		};

		std::for_each(fromBeginOfInsertedVertices, toItsEnd, modifyShaderID);
	}

	void FMeshBatchStatic::submitIndices(const FIndicesArray& indices) {
		p_indices.insert(p_indices.end(), indices.begin(), indices.end());

		auto fromBeginOfInsertedIndices = p_indices.end() - indices.size();
		auto toItsEnd = p_indices.end();
		auto extendIndices = [this](uint32_t& indice) {
			indice += p_indicesMaxValue;
		};

		std::for_each(fromBeginOfInsertedIndices, toItsEnd, extendIndices);
	}

	void FMeshBatchStatic::submitTransform(const TransformComponent& transformComponent) {
		p_transforms.emplace_back(transformComponent.getTransform());
	}

	const FVertexArray& FMeshBatchStatic::getVertices() const {
		return p_vertices;
	}

	const FIndicesArray& FMeshBatchStatic::getIndices() const {
		return p_indices;
	}

	const FTransformsArray& FMeshBatchStatic::getTransforms() const {
		return p_transforms;
	}

	uint32_t FMeshBatchStatic::getUniquePipelineID() const {
		return p_uniquePipelineID;
	}

	void FMeshBatchStatic::setUniquePipelineID(uint32_t id) {
		p_uniquePipelineID = id;
	}

	uint32_t FMeshBatchStatic::getUniqueTransformsID() const {
		return p_transformsUniqueID;
	}

	void FMeshBatchStatic::seUniqueTransformsID(uint32_t id) {
		p_transformsUniqueID = id;
	}



	void FMeshBatchStaticColor::reset() {
		FMeshBatchStatic::reset();

		m_colors.clear();
	}

	bool FMeshBatchStaticColor::canBeBatched(const Entity& entity) const {
		const bool baseClassPermission{ FMeshBatchStatic::canBeBatched(entity) };

		if (baseClassPermission && entity.hasComponent<ColorComponent>()) {
			return true;
		}
		else {
			return false;
		}
	}

	void FMeshBatchStaticColor::submitToBatch(const Entity& entity) {
		FMeshBatchStatic::submitToBatch(entity);

		submitColor(entity.getComponent<ColorComponent>());

		auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
		meshBatchInfoComponent.indexAtBatch = p_transforms.size() - 1;
	}

	void FMeshBatchStaticColor::submitColor(const ColorComponent& colorComponent) {
		m_colors.emplace_back(colorComponent.color);
	}

	const FColorsArray& FMeshBatchStaticColor::getColors() const {
		return m_colors;
	}

	uint32_t FMeshBatchStaticColor::getUniqueColorsID() const {
		return m_uniqueColorsID;
	}

	void FMeshBatchStaticColor::setUniqueColorsID(uint32_t id) {
		m_uniqueColorsID = id;
	}

	EBatchType FMeshBatchStaticColor::getBatchType() const {
		return EBatchType::MESH_STATIC_COLOR;
	}



	void FMeshBatchStaticTexture2D::reset() {
		FMeshBatchStatic::reset();

		m_textures.clear();
	}

	bool FMeshBatchStaticTexture2D::canBeBatched(const Entity& entity) const {
		const bool baseClassPermission{ FMeshBatchStatic::canBeBatched(entity) };

		if (baseClassPermission && entity.hasComponent<Texture2DComponent>()) {
			return true;
		}
		else {
			return false;
		}
	}

	void FMeshBatchStaticTexture2D::submitToBatch(const Entity& entity) {
		FMeshBatchStatic::submitToBatch(entity);

		const uint32_t bindingIndex{ ((uint32_t)p_shapeID - 1) };
		submitTexture(bindingIndex, entity.getComponent<Texture2DComponent>());

		auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
		meshBatchInfoComponent.indexAtBatch = p_transforms.size() - 1;
	}

	void FMeshBatchStaticTexture2D::submitTexture(uint32_t bindingIndex, const Texture2DComponent& textureComponent) {
		m_textures.emplace_back(bindingIndex, textureComponent.texturePath);
	}

	const FTexturesArray& FMeshBatchStaticTexture2D::getTextures() const {
		return m_textures;
	}

	EBatchType FMeshBatchStaticTexture2D::getBatchType() const {
		return EBatchType::MESH_STATIC_TEX2D;
	}



}
