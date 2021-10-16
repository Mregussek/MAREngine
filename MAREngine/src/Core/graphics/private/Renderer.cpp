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


#include "../public/Renderer.h"
#include "../public/BatchManager.h"
#include "../../ecs/SceneManagerEditor.h"
#include "../../ecs/Scene.h"


namespace marengine {


    void FRenderCommand::create(FRenderStatistics* pRenderStatistics) {
        p_pRenderStatistics = pRenderStatistics;
    }


    void FRenderStatistics::create(FBatchManager* pBatchManager) {
        m_pBatchManager = pBatchManager;
    }

    template<typename TBatchStorageType>
    void updateStatisticsStoragePerBatch(const TBatchStorageType* pBatchStorage, FRenderStatsStorage& statsStorage,
                                         uint32& batchEntitiesCount) {
        const uint32 count{ pBatchStorage->getCount() };
        for(uint32 i = 0; i < count; i++) {
            const auto pBatch{ pBatchStorage->get((int32) i) };
            statsStorage.verticesCount += pBatch->getVertices().size();
            statsStorage.indicesCount += pBatch->getIndices().size();
            statsStorage.trianglesCount += (statsStorage.indicesCount / 3);
            batchEntitiesCount += pBatch->getTransforms().size();
            statsStorage.allRendererEntitiesCount += statsStorage.coloredEntitiesCount;
        }
    }

    void FRenderStatistics::update(FSceneManagerEditor* pSceneManagerEditor) {
        const FMeshBatchStorage* pBatchStorage{ m_pBatchManager->getMeshBatchStorage() };

        updateStatisticsStoragePerBatch(pBatchStorage->getStorageStaticColor(), m_storage, m_storage.coloredEntitiesCount);
        updateStatisticsStoragePerBatch(pBatchStorage->getStorageStaticTex2D(), m_storage, m_storage.textured2dEntitiesCount);

        m_storage.entitiesCount = pSceneManagerEditor->getScene()->getEntities().size();
    }

    void FRenderStatistics::reset() {
        m_storage.drawCallsCount = 0;
        m_storage.verticesCount = 0;
        m_storage.indicesCount = 0;
        m_storage.trianglesCount = 0;
        m_storage.entitiesCount = 0;
        m_storage.coloredEntitiesCount = 0;
        m_storage.textured2dEntitiesCount = 0;
        m_storage.allRendererEntitiesCount = 0;
    }

    FRenderStatsStorage& FRenderStatistics::getStorage() {
        return m_storage;
    }


}
