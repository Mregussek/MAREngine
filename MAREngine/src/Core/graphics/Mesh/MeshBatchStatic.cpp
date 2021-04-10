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


#include "MeshBatch.h"
#include "Mesh.h"
#include "../IRender.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


    void FMeshBatchStatic::reset() {
        FMeshBatch::reset();
        p_shapeID = 0.f;
        p_indicesMaxValue = 0;
    }

    bool FMeshBatchStatic::shouldBeBatched(const Entity& entity) const {
        if(!entity.hasComponent<RenderableComponent>() ||
                !entity.hasComponent<MeshBatchInfoComponent>()) {
            return false;
        }

        const auto& renderable{ entity.getComponent<RenderableComponent>() };
        if(renderable.meshIndex == -1 || renderable.meshType == EMeshType::NONE) {
            return false;
        }

        return true;
    }

    bool FMeshBatchStatic::canBeBatched(const Entity& entity) const {
        const bool entityDoesntHaveRenderable{ !entity.hasComponent<RenderableComponent>() };
        if (entityDoesntHaveRenderable) {
            return false;
        }

        const auto& renderableComponent{ entity.getComponent<RenderableComponent>() };
        const FMeshProxy* pMesh{ p_pMeshStorage->retrieve(renderableComponent) };
        if(pMesh == nullptr) {
            return false;
        }

        const size_t verticesToPush{ pMesh->getVertices().size() };
        const size_t indicesToPush{ pMesh->getIndices().size() };

        const size_t currentVerticesSize{ p_vertices.size() };
        const size_t currentIndicesSize{ p_indices.size() };
        const size_t currentTransformSize{ p_transforms.size() };

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
        submitRenderable(entity.getComponent<RenderableComponent>());
        submitTransform(entity.getComponent<TransformComponent>());

        auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
        meshBatchInfoComponent.indexAtBatch = (int8)(p_transforms.size() - 1);
    }

    void FMeshBatchStatic::submitRenderable(const RenderableComponent& renderableComponent) {
        const FMeshProxy* pMesh{ p_pMeshStorage->retrieve(renderableComponent) };
        const FVertexArray& vertices{ pMesh->getVertices() };
        submitVertices(vertices);
        submitIndices(pMesh->getIndices());

        p_indicesMaxValue += (vertices.size() * sizeof(Vertex) / 4) / g_MeshStride;
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
        const RenderableComponent& renderable{ entity.getComponent<RenderableComponent>() };
        submitColor(renderable.color);

        auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
        meshBatchInfoComponent.batchIndex = getIndex();
        meshBatchInfoComponent.batchType = EBatchType::MESH_STATIC_COLOR;
    }

    void FMeshBatchStaticColor::submitColor(const maths::vec4& color) {
        m_colors.emplace_back(color);
    }

    const FColorsArray& FMeshBatchStaticColor::getColors() const {
        return m_colors;
    }

    int8 FMeshBatchStaticColor::getColorSSBO() const {
        return m_colorsSSBO;
    }

    void FMeshBatchStaticColor::setColorSSBO(int8 id) {
        m_colorsSSBO = id;
    }


}