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


namespace marengine {


    void FMaterialManager::create(FRenderContext* pRenderContext) {
        m_pMaterialFactory = pRenderContext->getMaterialFactory();
        m_pMaterialStorage = pRenderContext->getMaterialStorage();
    }

    void FMaterialManager::updateSceneMaterialData(Scene* pScene) {

    }

    void FMaterialManager::updateEntityMaterialData(const Entity& entity) const {

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
