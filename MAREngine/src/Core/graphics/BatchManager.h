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


#ifndef MARENGINE_BATCHMANAGER_H
#define MARENGINE_BATCHMANAGER_H


#include "IRender.h"
#include "Mesh.h"
#include "MeshBatch.h"
#include "Lightning/PointLightBatch.h"


namespace marengine {

    class Scene;
    class Entity;
    class FRenderManager;
    class FMeshManager;


    class FBatchManager : public IRenderResourceManager {
    public:

        void create(FRenderManager* pRenderManager, FMeshManager* pMeshManager);
        void reset();

        void pushSceneToRender(Scene* pScene);
        void pushEntityToRender(const Entity& entity);

        template<typename TComponent>
        void update(const Entity& entity) const { }

        MAR_NO_DISCARD FMeshBatchStorage* getMeshBatchStorage() const;
        MAR_NO_DISCARD FMeshBatchFactory* getMeshBatchFactory() const;
        MAR_NO_DISCARD FPointLightBatch* getPointLightBatch() const;

    private:

        FMeshBatchFactory m_meshBatchFactory;
        FPointLightBatch m_pointLightBatch;
        FRenderManager* m_pRenderManager{ nullptr };
        FMeshManager* m_pMeshManager{ nullptr };

    };



    template<> void FBatchManager::update<CRenderable>(const Entity& entity) const;
    template<> void FBatchManager::update<CTransform>(const Entity& entity) const;


}


#endif //MARENGINE_BATCHMANAGER_H
