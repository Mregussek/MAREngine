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


#include "../public/MaterialManager.h"
#include "../public/Material.h"
#include "../../ecs/Scene.h"
#include "../../../Logging/Logger.h"
#include "../../filesystem/public/FileManager.h"


namespace marengine {


    void FMaterialManager::create(FRenderContext* pRenderContext) {
        m_pMaterialFactory = pRenderContext->getMaterialFactory();
        m_pMaterialStorage = pRenderContext->getMaterialStorage();
    }

    void FMaterialManager::updateSceneMaterialData(Scene* pScene) {
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Pushing scene {} to material update...", pScene->getName());
        reset();
        const FEntityArray& entities{ pScene->getEntities() };
        for(const Entity& entity : entities) {
            updateEntityMaterialData(entity);
        }
        MARLOG_INFO(ELoggerType::GRAPHICS, "Pushed scene {} to material update!", pScene->getName());
    }

    void FMaterialManager::updateEntityMaterialData(const Entity& entity) const {
        const std::string& entityTag{ entity.getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Updating entity {} material data...", entityTag);
        if (!entity.hasComponent<CRenderable>()) {
            MARLOG_INFO(ELoggerType::GRAPHICS, "Entity {} has not material data!", entityTag);
            return;
        }

        auto& cRenderable{ entity.getComponent<CRenderable>() };
        if(FFileManager::isContainingExtension(cRenderable.material.path, "jpg")) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Trying to load new .jpg file as entity {} has cRenderable material = ", entityTag, cRenderable.material.path);
            const FMaterialProxy* pMaterial{ getStorage()->isAlreadyLoadedTex2D(cRenderable.material.path) };
            if(pMaterial == nullptr) {
                MARLOG_DEBUG(ELoggerType::GRAPHICS, "Loading texture {} assigned to entity {}...", cRenderable.material.path, entityTag);
                pMaterial = getFactory()->emplaceTex2D(cRenderable.material.path);
            }
            cRenderable.material.index = pMaterial->getIndex();
            cRenderable.material.type = EMaterialType::TEX2D;
            MARLOG_INFO(ELoggerType::GRAPHICS, "Updated entity {} texture data! Texture: {}", entityTag, cRenderable.material.path);
        }
        else {
            MARLOG_INFO(ELoggerType::GRAPHICS, "No need to update material as entity {} uses color! Texture: {}", entityTag, cRenderable.material.path);
        }

    }

    void FMaterialManager::reset() {
        m_pMaterialStorage->reset();
    }

    FMaterialStorage* FMaterialManager::getStorage() const {
        return m_pMaterialStorage;
    }

    FMaterialFactory* FMaterialManager::getFactory() const {
        return m_pMaterialFactory;
    }


}
