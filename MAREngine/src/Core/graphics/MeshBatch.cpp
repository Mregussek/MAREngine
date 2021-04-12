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


namespace marengine {


	void FMeshBatch::reset() {
		p_vertices.clear();
		p_indices.clear();
		p_transforms.clear();
        p_vbo = p_ibo = p_transformSSBO = -1;
	}


    FMeshBatchStaticColor* FMeshBatchStorage::getStaticColor(int8 index) const {
	    return const_cast<FMeshBatchStaticColor*>(&m_staticColors.at(index));
	}

    FMeshBatchStaticTex2D* FMeshBatchStorage::getStaticTex2D(int8 index) const {
        return const_cast<FMeshBatchStaticTex2D*>(&m_staticTex2D.at(index));
    }

    size_t FMeshBatchStorage::getCountStaticColor() const {
	    return m_staticColors.size();
	}

    size_t FMeshBatchStorage::getCountStaticTex2D() const {
	    return m_staticTex2D.size();
	}

	template<typename TBatchArray>
	static void clearBatchArray(TBatchArray& array) {
        for(auto& batch : array) {
            batch.reset();
        }
        array.clear();
	}

	void FMeshBatchStorage::reset() {
        clearBatchArray(m_staticColors);
        clearBatchArray(m_staticTex2D);
	}

    auto FMeshBatchStorage::getArrayStaticColor() ->decltype(&m_staticColors) const {
	    return &m_staticColors;
	}

    auto FMeshBatchStorage::getArrayStaticTex2D() ->decltype(&m_staticTex2D) const {
	    return &m_staticTex2D;
	}


    FMeshBatchStaticColor* FMeshBatchFactory::emplaceStaticColor() {
        auto& batch{ m_storage.m_staticColors.emplace_back() };
        const int8 currentSize{ (int8)m_storage.getCountStaticColor() };
        batch.setIndex(currentSize - 1);
        batch.passStorage(m_pMeshStorage);
        return const_cast<FMeshBatchStaticColor*>(&batch);
	}

    FMeshBatchStaticTex2D* FMeshBatchFactory::emplaceStaticTex2D() {
        auto& batch{ m_storage.m_staticTex2D.emplace_back() };
        const int8 currentSize{ (int8)m_storage.getCountStaticTex2D() };
        batch.setIndex(currentSize - 1);
        batch.passStorage(m_pMeshStorage);
        return const_cast<FMeshBatchStaticTex2D*>(&batch);
	}

	FMeshBatchStorage* FMeshBatchFactory::getStorage() const {
	    return const_cast<FMeshBatchStorage*>(&m_storage);
	}


}
