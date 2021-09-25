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
#include "../../../Logging/Logger.h"


namespace marengine {


    void FBatchManager::create(FRenderManager* pRenderManager, FMeshStorage* pMeshStorage,
                               FMaterialStorage* pMaterialStorage) {
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Creating Batch Manager...");
        m_pRenderManager = pRenderManager;
        m_meshBatchFactory.passMeshStorage(pMeshStorage);
        m_meshBatchFactory.passMaterialStorage(pMaterialStorage);
    }

    void FBatchManager::reset() const {
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Resetting Batch Manager...");
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
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Pushing scene {} to render...", pScene->getName());
        m_pRenderManager->reset();
        reset();

        const FEntityArray& entities{ pScene->getEntities() };
        for(const Entity& entity : entities) {
            pushEntityToRender(entity);
        }
        m_pRenderManager->onBatchesReadyToDraw(this);
        MARLOG_INFO(ELoggerType::GRAPHICS, "Pushed scene {} to render!", pScene->getName());
    }

    template<typename TMeshBatchArray>
    static int32 getAvailableBatch(TMeshBatchArray* pBatchArray, const Entity& entity);

    template<typename TMeshBatchStorage>
    static bool pushEntityToBatchStorage(TMeshBatchStorage* pMeshBatchStorage,
                                         FMeshBatchFactory* pFactory,
                                         const Entity& entity);

    void FBatchManager::pushEntityToRender(const Entity& entity) {
        const std::string& entityTag{ entity.getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Push entity {} to render...", entityTag);
        if(entity.hasComponent<CRenderable>()) {
            MARLOG_TRACE(ELoggerType::GRAPHICS, "Entity {} has CRenderable, trying to push assigned mesh and material...",
                         entityTag);
            [&entity, &entityTag, this]() {
                MARLOG_TRACE(ELoggerType::GRAPHICS, "Validating Tex2D MeshBatchStorage for {} entity...", entityTag);
                if(pushEntityToBatchStorage(getMeshBatchStorage()->getStorageStaticTex2D(),
                                            getMeshBatchFactory(), entity)) {
                    MARLOG_DEBUG(ELoggerType::GRAPHICS, "Pushed entity {} to MeshBatch with Tex2D", entityTag);
                    return;
                }
                MARLOG_TRACE(ELoggerType::GRAPHICS, "Validating Color MeshBatchStorage for {} entity...", entityTag);
                if(pushEntityToBatchStorage(getMeshBatchStorage()->getStorageStaticColor(),
                                            getMeshBatchFactory(), entity)) {
                    MARLOG_DEBUG(ELoggerType::GRAPHICS, "Pushed entity {} to MeshBatch with Color", entityTag);
                    return;
                }
            }();
        }

        if (entity.hasComponent<CPointLight>()) {
            MARLOG_TRACE(ELoggerType::GRAPHICS, "Entity {} has CPointLight, trying to push light...", entityTag);
            FPointLightBatch* pPointLightBatch{ getLightBatchStorage()->getPointLightBatch() };
            if (pPointLightBatch->canBeBatched(entity)) {
                pPointLightBatch->submitToBatch(entity);
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "Pushed PointLight from entity {}!", entityTag);
            }
            else {
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "Could not push PointLight from entity {}!", entityTag);
            }
        }

        if (entity.hasComponent<CCamera>()) {
            MARLOG_TRACE(ELoggerType::GRAPHICS, "Entity {} has CCamera, trying to push Camera...", entityTag);
            auto& cameraComponent{ entity.getComponent<CCamera>() };
            const bool isMain{ cameraComponent.isMainCamera() };
            if (m_pRenderManager->isCameraValid() && isMain) {
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "{} entity's camera is main one, updating...", entityTag);
                FEventsCameraEntity::onMainCameraUpdate(entity);
            }
            else if (isMain) {
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "{} entity's camera is main one, pushing...", entityTag);
                const auto& transformComponent{ entity.getComponent<CTransform>() };
                FRenderCamera* renderCamera{ &cameraComponent.renderCamera };
                renderCamera->calculateCameraTransforms(transformComponent, cameraComponent);
                m_pRenderManager->setCamera(renderCamera);
            }
            else {
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "{} entity's camera is not main one...", entityTag);
            }
        }
    }


    template<typename TMeshBatchArray>
    int32 getAvailableBatch(TMeshBatchArray* pBatchArray, const Entity& entity) {
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Looking for available batch for an entity...");
        auto canBatchEntity = [&entity](const auto& batch)->bool {
            return batch.canBeBatched(entity);
        };
        const auto validBatchIt = std::find_if(pBatchArray->cbegin(), pBatchArray->cend(),
                                               canBatchEntity);
        if (validBatchIt != pBatchArray->cend()) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Found available batch, returning...");
            return std::distance(pBatchArray->cbegin(), validBatchIt);
        }

        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Could not find available batch, returning...");
        return -1;
    }

    template<typename TMeshBatchStorage>
    static bool pushEntityToBatchStorage(TMeshBatchStorage* pMeshBatchStorage,
                                         FMeshBatchFactory* pFactory,
                                         const Entity& entity) {
        const std::string& entityTag{ entity.template getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Trying to push entity {} to batch storage", entityTag);
        if(pMeshBatchStorage->isEmpty()) {
            MARLOG_TRACE(ELoggerType::GRAPHICS, "Adding new MeshBatch to MeshBatchStorage as it is empty...");
            FMeshBatchStatic* pBatch{ pFactory->emplaceStatic(pMeshBatchStorage) };
        }

        const int32 index{ getAvailableBatch(pMeshBatchStorage->getArray(), entity) };
        if (index != -1) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Found available batch, pushing entity {}", entityTag);
            pMeshBatchStorage->get(index)->submitToBatch(entity);
            return true;
        }
        else {
            MARLOG_TRACE(ELoggerType::GRAPHICS, "Checking, if entity {} should be batched at first MeshBatch...",
                         entityTag);
            if (pMeshBatchStorage->get(0)->shouldBeBatched(entity)) {
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "Entity {} should be batched, creating new batch and submitting...",
                             entityTag);
                FMeshBatchStatic* pBatch{ pFactory->emplaceStatic(pMeshBatchStorage) };
                pBatch->submitToBatch(entity);
                return true;
            }
        }

        MARLOG_WARN(ELoggerType::GRAPHICS, "Could not push entity {} to batch storage...", entityTag);
        return false;
    }


}
