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


#include "SceneLayer.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity/EventsCameraEntity.h"
#include "../../Core/ecs/Entity/EventsComponentEntity.h"
#include "../../Core/filesystem/SceneDeserializer.h"
#include "../../Core/graphics/public/BatchManager.h"
#include "../../Core/graphics/public/MeshManager.h"
#include "../../Core/graphics/public/RenderManager.h"
#include "../../Core/graphics/public/Framebuffer.h"


namespace marengine {


	void FSceneLayer::create(const std::string& scenePath, FRenderManager* pRenderManager,
                             FBatchManager* pBatchManager, FMeshManager* pMeshManager) {
        Scene* pScene = FSceneDeserializer::loadSceneFromFile(scenePath);

        pRenderManager->getViewportFramebuffer()->setClearColor(pScene->getBackground());

        FEventsCameraEntity::passSceneManager(&m_sceneManagerEditor);
        FEventsCameraEntity::passRenderManager(pRenderManager);

        FEventsComponentEntity::passSceneManager(&m_sceneManagerEditor);
        FEventsComponentEntity::passRenderManager(pRenderManager);
        FEventsComponentEntity::passBatchManager(pBatchManager);
        FEventsComponentEntity::passMeshManager(pMeshManager);

        m_sceneManagerEditor.initialize(pScene, pBatchManager, pMeshManager);
	}

	void FSceneLayer::begin() {

	}

	void FSceneLayer::update() {
        m_sceneManagerEditor.update();
	}

	void FSceneLayer::end() {

	}

	void FSceneLayer::close() {
        m_sceneManagerEditor.close();
	}

	FSceneManagerEditor* FSceneLayer::getSceneManager() {
		return &m_sceneManagerEditor;
	}


}
