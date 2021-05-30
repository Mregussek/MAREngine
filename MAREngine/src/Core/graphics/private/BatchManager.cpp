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


#include "../public/BatchManager.h"
#include "../public/RenderManager.h"
#include "../public/MeshManager.h"
#include "../public/MaterialManager.h"
#include "../../ecs/Entity/EventsCameraEntity.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Scene.h"


namespace marengine {


    void FBatchManager::create(FRenderManager* pRenderManager, FMeshStorage* pMeshStorage,
                               FMaterialStorage* pMaterialStorage) {
        m_pRenderManager = pRenderManager;
        m_meshBatchFactory.passMeshStorage(pMeshStorage);
        m_meshBatchFactory.passMaterialStorage(pMaterialStorage);
    }

    void FBatchManager::reset() const {
        getMeshBatchStorage()->reset();
        getLightBatchStorage()->reset();
    }

    FMeshBatchStorage* FBatchManager::getMeshBatchStorage() const {
        return const_cast<FMeshBatchStorage*>(m_meshBatchFactory.getStorage());
    }

    FMeshBatchFactory* FBatchManager::getMeshBatchFactory() const {
        return const_cast<FMeshBatchFactory*>(&m_meshBatchFactory);
    }

    FLightBatchFactory* FBatchManager::getLightBatchFactory() const {
        return const_cast<FLightBatchFactory*>(&m_lightFactory);
    }

    FLightBatchStorage* FBatchManager::getLightBatchStorage() const {
        return const_cast<FLightBatchStorage*>(m_lightFactory.getStorage());
    }

    void FBatchManager::pushSceneToRender(Scene* pScene) {
        m_pRenderManager->reset();
        reset();

        const FEntityArray& entities{ pScene->getEntities() };
        for(const Entity& entity : entities) {
            pushEntityToRender(entity);
        }
        m_pRenderManager->onBatchesReadyToDraw(this);
    }

    template<typename TMeshBatchArray>
    static int32 getAvailableBatch(TMeshBatchArray* pBatchArray, const Entity& entity);

    template<typename TMeshBatchStorage>
    static bool pushEntityToBatchStorage(TMeshBatchStorage* pMeshBatchStorage,
                                         FMeshBatchFactory* pFactory,
                                         const Entity& entity);

    void FBatchManager::pushEntityToRender(const Entity& entity) {
        if(entity.hasComponent<CRenderable>()) {
            [&entity, this]() {
                if(pushEntityToBatchStorage(getMeshBatchStorage()->getStorageStaticTex2D(),
                                            getMeshBatchFactory(), entity)) { return; }
                if(pushEntityToBatchStorage(getMeshBatchStorage()->getStorageStaticColor(),
                                            getMeshBatchFactory(), entity)) { return; }
            }();
        }

        if (entity.hasComponent<CPointLight>()) {
            FPointLightBatch* pPointLightBatch{ getLightBatchStorage()->getPointLightBatch() };
            if (pPointLightBatch->canBeBatched(entity)) {
                pPointLightBatch->submitToBatch(entity);
            }
        }

        if (entity.hasComponent<CCamera>()) {
            auto& cameraComponent{ entity.getComponent<CCamera>() };
            const bool isMain{ cameraComponent.isMainCamera() };
            if (m_pRenderManager->isCameraValid() &&isMain) {
                FEventsCameraEntity::onMainCameraUpdate(entity);
            }
            else if(isMain){
                const auto& transformComponent{ entity.getComponent<CTransform>() };
                FRenderCamera* renderCamera{ &cameraComponent.renderCamera };
                renderCamera->calculateCameraTransforms(transformComponent, cameraComponent);
                m_pRenderManager->setCamera(renderCamera);
            }
        }
    }


    template<typename TMeshBatchArray>
    int32 getAvailableBatch(TMeshBatchArray* pBatchArray, const Entity& entity) {
        auto canBatchEntity = [&entity](const auto& batch)->bool {
            return batch.canBeBatched(entity);
        };
        const auto validBatchIt = std::find_if(pBatchArray->cbegin(), pBatchArray->cend(),
                                               canBatchEntity);
        if (validBatchIt != pBatchArray->cend()) {
            return std::distance(pBatchArray->cbegin(), validBatchIt);
        }

        return -1;
    }

    template<typename TMeshBatchStorage>
    static bool pushEntityToBatchStorage(TMeshBatchStorage* pMeshBatchStorage,
                                         FMeshBatchFactory* pFactory,
                                         const Entity& entity) {
        if(pMeshBatchStorage->isEmpty()) {
            FMeshBatchStatic* pBatch{ pFactory->emplaceStatic(pMeshBatchStorage) };
        }

        const int32 index = getAvailableBatch(pMeshBatchStorage->getArray(), entity);
        if (index != -1) {
            pMeshBatchStorage->get(index)->submitToBatch(entity);
            return true;
        }
        else {
            if (pMeshBatchStorage->get(0)->shouldBeBatched(entity)) {
                FMeshBatchStatic* pBatch{ pFactory->emplaceStatic(pMeshBatchStorage) };
                pBatch->submitToBatch(entity);
                return true;
            }
        }

        return false;
    }


}
