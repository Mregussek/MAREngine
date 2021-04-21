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
#include "../../ecs/Entity/EventsCameraEntity.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Scene.h"


namespace marengine {

    template<typename TMeshBatchArray>
    static int32 getAvailableBatch(TMeshBatchArray* pBatchArray, const Entity& entity);


    void FBatchManager::create(FRenderManager* pRenderManager, FMeshManager* pMeshManager) {
        m_pRenderManager = pRenderManager;
        m_pMeshManager = pMeshManager;
        m_meshBatchFactory.passMeshStorage(m_pMeshManager->getStorage());
    }

    void FBatchManager::reset() {
        getMeshBatchStorage()->reset();
        m_pointLightBatch.reset();
    }

    FMeshBatchStorage* FBatchManager::getMeshBatchStorage() const {
        return const_cast<FMeshBatchStorage*>(m_meshBatchFactory.getStorage());
    }

    FMeshBatchFactory* FBatchManager::getMeshBatchFactory() const {
        return const_cast<FMeshBatchFactory*>(&m_meshBatchFactory);
    }

    FPointLightBatch* FBatchManager::getPointLightBatch() const {
        return const_cast<FPointLightBatch*>(&m_pointLightBatch);
    }

    void FBatchManager::pushSceneToRender(Scene* pScene) {
        m_pRenderManager->reset();
        reset();

        auto inst1 = m_meshBatchFactory.emplaceStaticColor();
        auto inst2 = m_meshBatchFactory.emplaceStaticTex2D();

        const FEntityArray& entities{ pScene->getEntities() };
        for(const Entity& entity : entities) {
            pushEntityToRender(entity);
        }
        m_pRenderManager->onBatchesReadyToDraw(this);
    }

    void FBatchManager::pushEntityToRender(const Entity& entity) {
        // TODO: does not support more than one batch! Refactor it
        if(entity.hasComponent<CRenderable>()) {
            [this](const Entity& entity) -> bool {
                auto arrayColor{ getMeshBatchStorage()->getArrayStaticColor() };
                const int8 batchIndex = getAvailableBatch(arrayColor, entity);
                if (batchIndex != -1) {
                    getMeshBatchStorage()->getStaticColor(batchIndex)->submitToBatch(entity);
                    return true;
                }
                return false;
            }(entity);
        }

        if (entity.hasComponent<CPointLight>()) {
            if (m_pointLightBatch.canBeBatched(entity)) {
                m_pointLightBatch.submitEntityWithLightning(entity);
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
                RenderCamera* renderCamera{ &cameraComponent.renderCamera };
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
        const auto validBatchIt = std::find_if(pBatchArray->cbegin(), pBatchArray->cend(), canBatchEntity);
        if (validBatchIt != pBatchArray->cend()) {
            return std::distance(pBatchArray->cbegin(), validBatchIt);
        }

        return -1;
    }


}
