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
#include "../SceneManagerEditor.h"
#include "../Components/Components.h"
#include "../../graphics/Mesh/EventsMeshBatchStatic.h"
#include "../../graphics/Lightning/EventsLightBatch.h"
#include "../../graphics/RenderAPI/RenderBufferManager.h"


namespace marengine {


	/***************************** TRANSFORM COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onUpdate<TransformComponent>(const Entity& entity) {
		const auto& transform{ entity.getComponent<TransformComponent>() };
		const auto& meshBachInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };

		const bool isEntityRendered = [&meshBachInfoComponent]()->bool {
			const bool hasAssignedBatch{ meshBachInfoComponent.batchType != EMeshBatchStaticType::NONE };
			const bool indexAtBatchIsCorrect{ meshBachInfoComponent.batchIndex != -1 };
			return hasAssignedBatch && indexAtBatchIsCorrect;
		}();

		if (isEntityRendered) {
			FEventsMeshBatchStatic::onTransformUpdate(entity);
		}

		if (entity.hasComponent<CameraComponent>()) {
			const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
			if (cameraComponent.isMainCamera()) {
				FEventsCameraEntity::onMainCameraUpdate(entity);
			}
		}

		if (entity.hasComponent<PointLightComponent>()) {
			FEventsLightBatch::onPointLightPositionUpdate(entity);
		}
	}

	/***************************** RENDERABLE COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onAdd<RenderableComponent>(const Entity& entity) {
		entity.addComponent<RenderableComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<RenderableComponent>(const Entity& entity) {
		FSceneManagerEditor::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<RenderableComponent>(const Entity& entity) {
		entity.removeComponent<RenderableComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	/***************************** COLOR COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onAdd<ColorComponent>(const Entity& entity) {
		entity.addComponent<ColorComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<ColorComponent>(const Entity& entity) {
		FEventsMeshBatchStatic::onColorUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<ColorComponent>(const Entity& entity) {
		entity.removeComponent<ColorComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	/***************************** TEXTURE 2D COMPONENT TEMPLATES **************************************/

	template<> void FEventsComponentEntity::onUpdate<Texture2DComponent>(const Entity& entity) {
		FSceneManagerEditor::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<Texture2DComponent>(const Entity& entity) {
		entity.removeComponent<Texture2DComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	/***************************** TEXTURE CUBEMAP COMPONENT TEMPLATES *********************************/

	template<> void FEventsComponentEntity::onUpdate<TextureCubemapComponent>(const Entity& entity) {
		FSceneManagerEditor::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onRemove<TextureCubemapComponent>(const Entity& entity) {
		entity.removeComponent<TextureCubemapComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	/***************************** LIGHT COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onAdd<PointLightComponent>(const Entity& entity) {
		entity.addComponent<PointLightComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	template<> void FEventsComponentEntity::onUpdate<PointLightComponent>(const Entity& entity) {
		FEventsLightBatch::onPointLightUpdate(entity);
	}

	template<> void FEventsComponentEntity::onRemove<PointLightComponent>(const Entity& entity) {
		entity.removeComponent<PointLightComponent>();
		FSceneManagerEditor::Instance->initialize();
	}

	/***************************** CAMERA COMPONENT TEMPLATES ***************************************/

	template<> void FEventsComponentEntity::onRemove<CameraComponent>(const Entity& entity) {
		const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
		if (cameraComponent.isMainCamera()) {
			// cannot remove main camera!
		}
		else {
			entity.removeComponent<CameraComponent>();
		}
	}


}
