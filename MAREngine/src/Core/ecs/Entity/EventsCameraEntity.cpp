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
#include "../../graphics/public/RenderManager.h"


namespace marengine {

    FSceneManagerEditor* FEventsCameraEntity::s_pSceneManagerEditor{ nullptr };
	FRenderManager* FEventsCameraEntity::s_pRenderManager{ nullptr };


    void FEventsCameraEntity::passSceneManager(FSceneManagerEditor* pSceneManagerEditor) {
        s_pSceneManagerEditor = pSceneManagerEditor;
    }

    void FEventsCameraEntity::passRenderManager(FRenderManager* pRenderManager) {
        s_pRenderManager = pRenderManager;
    }

	void FEventsCameraEntity::onMainCameraUpdate(const Entity& entity) {
		auto& cCamera{ entity.getComponent<CCamera>() };

		const bool userCheckingGameInPlayMode{ s_pSceneManagerEditor->isPlayMode() || s_pSceneManagerEditor->isPauseMode() };
		const bool userModifyingGameCameraInEditorMode{ s_pSceneManagerEditor->isEditorMode() && s_pSceneManagerEditor->usingGameCamera() };

		if (userCheckingGameInPlayMode || userModifyingGameCameraInEditorMode) {
			const auto& transform{ entity.getComponent<CTransform>() };
            cCamera.renderCamera.calculateCameraTransforms(transform, cCamera);
			s_pRenderManager->pushCameraToRender(&cCamera.renderCamera);
		}
	}

	void FEventsCameraEntity::onEditorCameraSet(const FRenderCamera* pRenderCamera) {
		s_pRenderManager->pushCameraToRender(pRenderCamera);
	}

	void FEventsCameraEntity::onGameCameraSet() {
		Scene* pScene{ s_pSceneManagerEditor->getScene() };
		entt::entity foundEnttEntity{ entt::null };

		auto CCameraAndCheckWhichIsMainCamera = [&foundEnttEntity](entt::entity enttEntity,
		                                                           CCamera& cameraComponent) {
			if (cameraComponent.isMainCamera()) {
				foundEnttEntity = enttEntity;
				return;
			}
		};
		auto viewOver{ pScene->getView<CCamera>() };

		viewOver.each(CCameraAndCheckWhichIsMainCamera);

		if (pScene->isValid(foundEnttEntity)) {
			auto& cCamera{ pScene->getComponent<CCamera>(foundEnttEntity) };
			const auto& transformComponent{ pScene->getComponent<CTransform>(foundEnttEntity) };
			FRenderCamera* pRenderCamera{ &cCamera.renderCamera };

			pRenderCamera->calculateCameraTransforms(transformComponent, cCamera);
			s_pRenderManager->pushCameraToRender(pRenderCamera);
		}
	}


}
