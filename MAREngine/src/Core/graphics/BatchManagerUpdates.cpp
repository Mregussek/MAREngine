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


#include "BatchManager.h"
#include "MeshBatch.h"
#include "MeshManager.h"
#include "RenderManager.h"
#include "../ecs/Entity/Entity.h"


namespace marengine {


    template<> void FBatchManager::update<CRenderable>(const Entity& entity) const {
        const auto& cRenderable{ entity.getComponent<CRenderable>() };
        const auto& cEvent{ entity.getComponent<CEvent>() };

        if(cEvent.componentUpdateType == EComponentUpdateType::RENDERABLE_COLOR) {
            FMeshBatchStaticColor* pMeshBatch =
                    getMeshBatchStorage()->getStaticColor(cRenderable.batch.index);
           pMeshBatch->updateColor(entity);
           m_pRenderManager->update<ERenderBatchUpdateType::RENDERABLE_COLOR>(pMeshBatch);
        }
        else if(cEvent.componentUpdateType == EComponentUpdateType::RENDERABLE_MESH) {

        }
    }

    template<> void FBatchManager::update<CTransform>(const Entity& entity) const {
        FMeshBatch* pMeshBatch{ getMeshBatchStorage()->retrieve(entity.getComponent<CRenderable>()) };
        pMeshBatch->updateTransform(entity);
        m_pRenderManager->update<ERenderBatchUpdateType::TRANSFORM>(pMeshBatch);
    }

    template<> void FBatchManager::update<CPointLight>(const Entity& entity) const {
        FPointLightBatch* pLightBatch{ getPointLightBatch() };
        pLightBatch->updateLight(entity);
        m_pRenderManager->update<ERenderBatchUpdateType::POINTLIGHT>(pLightBatch);
    }


}
