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
#include "../../Core/graphics/Renderer/RenderManager.h"


namespace marengine {


	void FSceneLayer::create(const std::string& scenePath, FRenderManager* pRenderManager) {
        Scene* pScene = FSceneDeserializer::loadSceneFromFile(scenePath);

        FEventsCameraEntity::create(&m_sceneManagerEditor, pRenderManager);
        FEventsComponentEntity::create(&m_sceneManagerEditor);

        m_sceneManagerEditor.initialize(pScene, pRenderManager);
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
