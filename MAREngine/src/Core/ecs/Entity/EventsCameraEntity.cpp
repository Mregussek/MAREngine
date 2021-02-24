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


#include "EventsCameraEntity.h"
#include "Entity.h"
#include "../Scene.h"
#include "../SceneManagerEditor.h"
#include "../../graphics/RenderAPI/RenderBufferManager.h"
#include "../../graphics/RenderAPI/RenderPipeline.h"


namespace marengine {


	void FEventsCameraEntity::onMainCameraUpdate(const RenderCamera* pRenderCamera) {
		FRenderBufferManager::onRenderCameraUpdate(pRenderCamera);
	}

	void FEventsCameraEntity::onMainCameraUpdate(const Entity& entity) {
		auto& cameraComponent{ entity.getComponent<CameraComponent>() };

		auto updateCameraOperation = [&entity, &cameraComponent]() {
			const auto& transform{ entity.getComponent<TransformComponent>() };
			cameraComponent.renderCamera.calculateCameraTransforms(transform, cameraComponent);
			onMainCameraUpdate(&cameraComponent.renderCamera);
		};

		const bool userCheckingGameInPlayMode{ FSceneManagerEditor::Instance->isPlayMode() || FSceneManagerEditor::Instance->isPauseMode() };
		const bool userModifyingGameCameraInEditorMode{ FSceneManagerEditor::Instance->isEditorMode() && !FSceneManagerEditor::Instance->usingEditorCamera() };

		if (userCheckingGameInPlayMode || userModifyingGameCameraInEditorMode) {
			updateCameraOperation();
		}
	}

	void FEventsCameraEntity::onEditorCameraSet(const RenderCamera* pRenderCamera) {
		RenderPipeline::Instance->pushCameraToPipeline(pRenderCamera);
		onMainCameraUpdate(pRenderCamera);
	}

	void FEventsCameraEntity::onGameCameraSet() {
		Scene* pScene{ FSceneManagerEditor::Instance->getScene() };
		entt::entity foundEnttEntity{ entt::null };

		auto entityWithCameraComponentAndLookForMainRenderCamera = [&foundEnttEntity](entt::entity enttEntity, CameraComponent& cameraComponent) {
			if (cameraComponent.isMainCamera()) {
				foundEnttEntity = enttEntity;
				return;
			}
		};
		auto viewOver{ pScene->getView<CameraComponent>() };

		viewOver.each(entityWithCameraComponentAndLookForMainRenderCamera);

		if (pScene->isValid(foundEnttEntity)) {
			auto& cameraComponent{ pScene->getComponent<CameraComponent>(foundEnttEntity) };
			const auto& transformComponent{ pScene->getComponent<TransformComponent>(foundEnttEntity) };
			RenderCamera* pRenderCamera{ &cameraComponent.renderCamera };

			pRenderCamera->calculateCameraTransforms(transformComponent, cameraComponent);
			RenderPipeline::Instance->pushCameraToPipeline(pRenderCamera);
			onMainCameraUpdate(pRenderCamera);
		}
	}


}
