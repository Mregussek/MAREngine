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


#include "../public/MeshBatch.h"
#include "../public/Mesh.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Entity/Components.h"


namespace marengine {


    void FMeshBatchStatic::reset() {
        FMeshBatch::reset();
        p_shapeID = 0.f;
        p_indicesMaxValue = 0;
    }

    bool FMeshBatchStatic::shouldBeBatched(const Entity& entity) const {
        if(!entity.hasComponent<CRenderable>()) {
            return false;
        }

        const auto& cRenderable{ entity.getComponent<CRenderable>() };
        if(cRenderable.mesh.index == -1 || cRenderable.mesh.type == EMeshType::NONE) {
            return false;
        }

        return true;
    }

    bool FMeshBatchStatic::canBeBatched(const Entity& entity) const {
        if (!entity.hasComponent<CRenderable>()) {
            return false;
        }

        const auto& cRenderable{ entity.getComponent<CRenderable>() };
        const FMeshProxy* pMesh{ p_pMeshStorage->retrieve(cRenderable) };
        if(pMesh == nullptr) {
            return false;
        }

        const uint32 verticesToPush{ pMesh->getVertices().size() };
        const uint32 indicesToPush{ pMesh->getIndices().size() };

        const uint32 currentVerticesSize{ p_vertices.size() };
        const uint32 currentIndicesSize{ p_indices.size() };
        const uint32 currentTransformSize{ p_transforms.size() };

        const bool cannotPushVertices =
                (currentVerticesSize + verticesToPush) >= GraphicLimits::maxVerticesCount;
        const bool cannotPushIndices =
                (currentIndicesSize + indicesToPush) >= GraphicLimits::maxIndicesCount;
        const bool cannotPushTransform =
                (currentTransformSize + 1) >= GraphicLimits::maxTransforms;

        const bool placeInBatchExist =
                !(cannotPushVertices || cannotPushIndices || cannotPushTransform);

        return placeInBatchExist; // true if there is place
    }

    void FMeshBatchStatic::submitToBatch(const Entity& entity) {
        auto& cRenderable{ entity.getComponent<CRenderable>() };
        submitRenderable(cRenderable);
        submitTransform(entity.getComponent<CTransform>());

        cRenderable.batch.index = getIndex();
        cRenderable.batch.transformIndex = (int32)(p_transforms.size() - 1);
    }

    void FMeshBatchStatic::submitRenderable(CRenderable& cRenderable) {
        const FMeshProxy* pMesh{ p_pMeshStorage->retrieve(cRenderable) };
        const FVertexArray& vertices{ pMesh->getVertices() };
        submitVertices(cRenderable, vertices);
        submitIndices(cRenderable, pMesh->getIndices());

        p_indicesMaxValue += (vertices.size() * sizeof(Vertex) / 4) / g_MeshStride;
        p_shapeID++;
    }

    void FMeshBatchStatic::submitVertices(CRenderable& cRenderable, const FVertexArray& vertices) {
        p_vertices.insert(p_vertices.end(), vertices.begin(), vertices.end());

        auto fromBeginOfInsertedVertices = p_vertices.end() - vertices.size();
        auto toItsEnd = p_vertices.end();
        auto modifyShaderID = [this](Vertex& vertex) {
            vertex.shapeID = p_shapeID;
        };

        std::for_each(fromBeginOfInsertedVertices, toItsEnd, modifyShaderID);

        cRenderable.batch.startVert = std::distance(p_vertices.begin(), fromBeginOfInsertedVertices);
        cRenderable.batch.endVert = std::distance(p_vertices.begin(), toItsEnd);
    }

    void FMeshBatchStatic::submitIndices(CRenderable& cRenderable, const FIndicesArray& indices) {
        p_indices.insert(p_indices.end(), indices.begin(), indices.end());

        auto fromBeginOfInsertedIndices = p_indices.end() - indices.size();
        auto toItsEnd = p_indices.end();
        auto extendIndices = [this](uint32_t& indice) {
            indice += p_indicesMaxValue;
        };

        std::for_each(fromBeginOfInsertedIndices, toItsEnd, extendIndices);

        cRenderable.batch.startInd = std::distance(p_indices.begin(), fromBeginOfInsertedIndices);
        cRenderable.batch.endInd = std::distance(p_indices.begin(), toItsEnd);
    }

    void FMeshBatchStatic::submitTransform(const CTransform& transformComponent) {
        p_transforms.emplace_back(transformComponent.getTransform());
    }


    void FMeshBatchStaticColor::reset() {
        FMeshBatchStatic::reset();
        m_colors.clear();
    }

    bool FMeshBatchStaticColor::shouldBeBatched(const Entity& entity) const {
        return FMeshBatchStatic::shouldBeBatched(entity);
    }

    bool FMeshBatchStaticColor::canBeBatched(const Entity& entity) const {
        const bool baseClassPermission{ FMeshBatchStatic::canBeBatched(entity) };
        if (!baseClassPermission) {
            return false;
        }

        return true;
    }

    void FMeshBatchStaticColor::submitToBatch(const Entity& entity) {
        FMeshBatchStatic::submitToBatch(entity);
        auto& cRenderable{ entity.getComponent<CRenderable>() };
        submitColor(cRenderable.color);

        cRenderable.batch.materialIndex = (int8)(m_colors.size() - 1);
        cRenderable.batch.type = EBatchType::MESH_STATIC_COLOR;
    }

    void FMeshBatchStaticColor::submitColor(const maths::vec4& color) {
        m_colors.emplace_back(color);
    }

    const FColorsArray& FMeshBatchStaticColor::getColors() const {
        return m_colors;
    }

    void FMeshBatchStaticColor::updateColor(const Entity& entity) {
        const auto& cRenderable{ entity.getComponent<CRenderable>() };
        m_colors.at(cRenderable.batch.materialIndex) = cRenderable.color;
    }

    int32 FMeshBatchStaticColor::getColorSSBO() const {
        return m_colorsSSBO;
    }

    void FMeshBatchStaticColor::passColorSSBO(int32 id) {
        m_colorsSSBO = id;
    }

    EBatchType FMeshBatchStaticColor::getType() const {
        return EBatchType::MESH_STATIC_COLOR;
    }


}