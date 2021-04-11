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


#include "EventsComponentEntity.inl"
#include "EventsCameraEntity.h"
#include "Components.h"
#include "../SceneManagerEditor.h"
#include "../../graphics/Mesh/EventsMeshBatchStatic.h"
#include "../../graphics/Lightning/EventsLightBatch.h"
#include "../../../Logging/Logger.h"


namespace marengine {

    FSceneManagerEditor* FEventsComponentEntity::s_pSceneManagerEditor{ nullptr };


    void FEventsComponentEntity::create(FSceneManagerEditor* pSceneManagerEditor) {
        s_pSceneManagerEditor = pSceneManagerEditor;
    }

	/***************************** TRANSFORM COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onUpdate<CTransform>(const Entity& entity) {
		const auto& cRenderable{ entity.getComponent<CRenderable>() };

		const bool isEntityRendered = [&cRenderable]()->bool {
			const bool hasAssignedBatch{ cRenderable.batch.type != EBatchType::NONE };
			const bool indexAtBatchIsCorrect{ cRenderable.batch.index != -1 };
			return hasAssignedBatch && indexAtBatchIsCorrect;
		}();

		if (isEntityRendered) {
			FEventsMeshBatchStatic::onTransformUpdate(entity);
		}

		if (entity.hasComponent<CCamera>() && entity.getComponent<CCamera>().isMainCamera()) {
			FEventsCameraEntity::onMainCameraUpdate(entity);
		}

		if (entity.hasComponent<CPointLight>()) {
			FEventsLightBatch::onPointLightPositionUpdate(entity);
		}
	}

	/***************************** RENDERABLE COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onAdd<CRenderable>(const Entity& entity) {
		entity.addComponent<CRenderable>();
		// TODO: implement better event that whole scene reinitialization
        s_pSceneManagerEditor->updateSceneAtBatchManager();
	}

	template<> void FEventsComponentEntity::onUpdate<CRenderable>(const Entity& entity) {
        // TODO: implement better event that whole scene reinitialization
        s_pSceneManagerEditor->updateSceneAtBatchManager();
	}

	template<> void FEventsComponentEntity::onRemove<CRenderable>(const Entity& entity) {
		entity.removeComponent<CRenderable>();
        // TODO: implement better event that whole scene reinitialization
        s_pSceneManagerEditor->updateSceneAtBatchManager();
	}

	/***************************** LIGHT COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onAdd<CPointLight>(const Entity& entity) {
		entity.addComponent<CPointLight>();
        // TODO: implement better event that whole scene reinitialization
        s_pSceneManagerEditor->updateSceneAtBatchManager();
	}

	template<> void FEventsComponentEntity::onUpdate<CPointLight>(const Entity& entity) {
		FEventsLightBatch::onPointLightUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<CPointLight>(const Entity& entity) {
		entity.removeComponent<CPointLight>();
        // TODO: implement better event that whole scene reinitialization
        s_pSceneManagerEditor->updateSceneAtBatchManager();
	}

	/***************************** CAMERA COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onRemove<CCamera>(const Entity& entity) {
		const auto& cCamera{ entity.getComponent<CCamera>() };
		if (cCamera.isMainCamera()) {
		    MARLOG_WARN(ELoggerType::ECS, "Cannot remove CCamera with MainCamera -> Entity: {}", entity.getComponent<CTag>().tag);
		}
		else {
			entity.removeComponent<CCamera>();
		}
	}


}
