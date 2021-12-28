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


#include "../public/MeshManager.h"
#include "../../filesystem/public/FileManager.h"
#include "../../ecs/Scene.h"
#include "../../../Logging/Logger.h"


namespace marengine {

    static bool isThereSubstring(const std::string& stringToCheck, const char* strToFind) {
        return stringToCheck.find(strToFind) != std::string::npos;
    }


    void FMeshManager::reset() {
        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Resetting MeshManager...");
        getStorage()->reset();
    }

    void FMeshManager::updateSceneMeshData(Scene* pScene) {
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Updating scene {} all meshes data...", pScene->getName());
        reset();
        auto view{ pScene->getView<CRenderable>() };
        view.each([this, pScene](entt::entity entt_entity, const CRenderable& cRenderable) {
            const Entity entity(entt_entity, pScene->getRegistry());
            updateEntityMeshData(entity);
        });
        MARLOG_INFO(ELoggerType::GRAPHICS, "Updated scene {} all meshes data!", pScene->getName());
    }

    void FMeshManager::updateEntityMeshData(const Entity& entity) const {
        const std::string& entityTag{ entity.getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Updating entity {} mesh data...", entityTag);

        auto& cRenderable{ entity.getComponent<CRenderable>() };

        if (isThereSubstring(cRenderable.mesh.path, "Cube")) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Set Cube as entity {} has cRenderable path = ", entityTag, cRenderable.mesh.path);
            cRenderable.mesh.index = g_MeshDefaultTypeIndex;
            cRenderable.mesh.type = EMeshType::CUBE;
        }
        else if (isThereSubstring(cRenderable.mesh.path, "Surface")) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Set Surface as entity {} has cRenderable path = ", entityTag, cRenderable.mesh.path);
            cRenderable.mesh.index = g_MeshDefaultTypeIndex;
            cRenderable.mesh.type = EMeshType::SURFACE;
        }
        else if (isThereSubstring(cRenderable.mesh.path, "Pyramid")) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Set Pyramid as entity {} has cRenderable path = ", entityTag, cRenderable.mesh.path);
            cRenderable.mesh.index = g_MeshDefaultTypeIndex;
            cRenderable.mesh.type = EMeshType::PYRAMID;
        }
        else if(FFileManager::isContainingExtension(cRenderable.mesh.path, "obj")) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Trying to load new .obj file as entity {} has cRenderable path = ", entityTag, cRenderable.mesh.path);
            const FMeshProxy* pMesh{ getStorage()->isAlreadyLoaded(cRenderable) };
            if(pMesh == nullptr) {
                pMesh = getFactory()->emplaceExternal(cRenderable.mesh.path);
            }
            cRenderable.mesh.index = pMesh->getIndex();
            cRenderable.mesh.type = EMeshType::EXTERNAL;
        }
        MARLOG_INFO(ELoggerType::GRAPHICS, "Updated entity {} mesh data!", entityTag);
    }

    FMeshStorage* FMeshManager::getStorage() const {
        return m_factory.getStorage();
    }

    FMeshFactory* FMeshManager::getFactory() const {
        return const_cast<FMeshFactory*>(&m_factory);
    }


}
