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
#include "../../../Logging/Logger.h"


namespace marengine {


	void FMeshBatch::reset() {
        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Resetting MeshBatch...");
		p_vertices.clear();
		p_indices.clear();
		p_transforms.clear();
        p_vbo = p_ibo = p_transformSSBO = -1;
	}

    const FVertexArray& FMeshBatch::getVertices() const {
	    return p_vertices;
	}

    const FIndicesArray& FMeshBatch::getIndices() const {
	    return p_indices;
	}

    const FTransformsArray& FMeshBatch::getTransforms() const {
	    return p_transforms;
	}

    void FMeshBatch::updateVertices(const Entity& entity) {
        // TODO: implement FMeshBatch::updateVertices
	    //const auto& cRenderable{ entity.getComponent<CRenderable>() };
	}

    void FMeshBatch::updateIndices(const Entity& entity) {
	    // TODO: implement FMeshBatch::updateIndices
        //const auto& cRenderable{ entity.getComponent<CRenderable>() };
	}

    void FMeshBatch::updateTransform(const Entity& entity) {
        p_transforms.at(entity.getComponent<CRenderable>().batch.transformIndex) =
                entity.getComponent<CTransform>().getTransform();
	}

    void FMeshBatch::passVBO(int32 index) {
	    p_vbo = index;
	}

    void FMeshBatch::passIBO(int32 index) {
	    p_ibo = index;
	}

    void FMeshBatch::passTransformSSBO(int32 index) {
	    p_transformSSBO = index;
	}

    int32 FMeshBatch::getVBO() const {
	    return p_vbo;
	}

    int32 FMeshBatch::getIBO() const {
	    return p_ibo;
	}

    int32 FMeshBatch::getTransformSSBO() const {
	    return p_transformSSBO;
	}

    void FMeshBatch::passMeshStorage(FMeshStorage* pMeshStorage) {
	    p_pMeshStorage = pMeshStorage;
	}

	void FMeshBatch::passMaterialStorage(FMaterialStorage* pMaterialStorage) {
        p_pMaterialStorage = pMaterialStorage;
	}


    FMeshBatchStaticColor* FMeshBatchStorageStaticColor::get(int32 index) const {
	    return const_cast<FMeshBatchStaticColor*>(&m_meshBatches.at(index));
	}

    uint32 FMeshBatchStorageStaticColor::getCount() const {
        return m_meshBatches.size();
    }

    bool FMeshBatchStorageStaticColor::isEmpty() const {
        return m_meshBatches.empty();
    }

    auto FMeshBatchStorageStaticColor::getArray() ->decltype(&m_meshBatches) const {
        return &m_meshBatches;
    }


    FMeshBatchStaticTex2D* FMeshBatchStorageStaticTex2D::get(int32 index) const {
        return const_cast<FMeshBatchStaticTex2D*>(&m_meshBatches.at(index));
    }

    uint32 FMeshBatchStorageStaticTex2D::getCount() const {
	    return m_meshBatches.size();
	}

    bool FMeshBatchStorageStaticTex2D::isEmpty() const {
        return m_meshBatches.empty();
    }

    auto FMeshBatchStorageStaticTex2D::getArray() ->decltype(&m_meshBatches) const {
        return &m_meshBatches;
    }


    FMeshBatchStorageStaticColor* FMeshBatchStorage::getStorageStaticColor() const {
	    return const_cast<FMeshBatchStorageStaticColor*>(&m_storageStaticColor);
	}

    FMeshBatchStorageStaticTex2D* FMeshBatchStorage::getStorageStaticTex2D() const {
        return const_cast<FMeshBatchStorageStaticTex2D*>(&m_storageStaticTex2D);
	}

    FMeshBatch* FMeshBatchStorage::retrieve(const CRenderable& cRenderable) const {
        switch(cRenderable.batch.type) {
            case EBatchType::MESH_STATIC_COLOR:
                return m_storageStaticColor.get(cRenderable.batch.index);
            case EBatchType::MESH_STATIC_TEX2D:
                return m_storageStaticTex2D.get(cRenderable.batch.index);
            default: return nullptr;
        }
    }

    template<typename TBatchArray>
	static void clearBatchArray(TBatchArray& array) {
        for(auto& batch : array) {
            batch.reset();
        }
        array.clear();
	}

	void FMeshBatchStorage::reset() {
        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Resetting MeshBatchStorage...");
        clearBatchArray(*m_storageStaticColor.getArray());
        clearBatchArray(*m_storageStaticTex2D.getArray());
	}


	template<typename TReturnType, typename TStorageType>
	static TReturnType* emplaceStorageType(TStorageType* pStorage, FMeshStorage* pMeshStorage,
                                FMaterialStorage* pMaterialStorage) {
        TReturnType* pBatch{ &pStorage->getArray()->emplace_back() };
        const auto currentSize{ pStorage->getCount() };
        pBatch->setIndex(currentSize - 1);
        pBatch->passMeshStorage(pMeshStorage);
        pBatch->passMaterialStorage(pMaterialStorage);
        return pBatch;
	}

    FMeshBatchStaticColor* FMeshBatchFactory::emplaceStaticColor() {
        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Adding new MeshBatch Color...");
        return emplaceStorageType<FMeshBatchStaticColor>(&m_storage.m_storageStaticColor,
                                                         m_pMeshStorage,
                                                         m_pMaterialStorage);
	}

    FMeshBatchStaticTex2D* FMeshBatchFactory::emplaceStaticTex2D() {
        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Adding new MeshBatch Tex2D...");
        return emplaceStorageType<FMeshBatchStaticTex2D>(&m_storage.m_storageStaticTex2D,
                                                         m_pMeshStorage,
                                                         m_pMaterialStorage);
	}

	FMeshBatchStorage* FMeshBatchFactory::getStorage() const {
	    return const_cast<FMeshBatchStorage*>(&m_storage);
	}

    void FMeshBatchFactory::passMeshStorage(FMeshStorage* pMeshStorage) {
	    m_pMeshStorage = pMeshStorage;
	}

	void FMeshBatchFactory::passMaterialStorage(FMaterialStorage* pMaterialStorage) {
	    m_pMaterialStorage = pMaterialStorage;
	}


}
