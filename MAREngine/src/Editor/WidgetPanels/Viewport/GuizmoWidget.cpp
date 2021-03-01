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


#include "GuizmoWidget.h"
#include "../../Camera/Camera.h"
#include "../../../Core/ecs/Entity/EventsComponentEntity.h"
#include "../../../Core/ecs/Components/DefaultComponents.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Window/Window.h"


namespace marengine {


	void GUI_Guizmo::selectType() {
		if (Window::isKeyPressed(MAR_KEY_LEFT_CONTROL)) {
			if (Window::isKeyPressed(MAR_KEY_Z)) { setTranslation(); }
			if (Window::isKeyPressed(MAR_KEY_X)) { setRotation(); }
			if (Window::isKeyPressed(MAR_KEY_C)) { setScale(); }
		}
	}

	void GUI_Guizmo::draw(const Camera& editorCamera, const Entity& currentEntity) const {
		auto& transform = currentEntity.getComponent<TransformComponent>();
		const bool userUsedGuizmo{ draw(editorCamera, transform) };

		if (userUsedGuizmo) {
			FEventsComponentEntity::onUpdate<TransformComponent>(currentEntity);
		}
	}

	bool GUI_Guizmo::draw(const Camera& editorCamera, TransformComponent& transformComponent) const {
		using namespace maths;

		mat4 transform{ transformComponent.getTransform() };
		float* pTransform{ transform.value_ptr_nonconst() };

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		const ImVec2 windowPos{ ImGui::GetWindowPos() };
		const ImVec2 windowSize{ ImGui::GetWindowSize() };
		ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

		const float* pView{ nullptr };
		const float* pProjection{ nullptr };
		{
			const RenderCamera* pRenderCam{ editorCamera.getCameraData() };
			pView = pRenderCam->getView().value_ptr();
			pProjection = pRenderCam->getProjection().value_ptr();
		}

		ImGuizmo::Manipulate(pView, pProjection, m_operation, ImGuizmo::MODE::LOCAL, pTransform);

		if (ImGuizmo::IsUsing()) {
			mat4::decompose(transform, transformComponent.position, transformComponent.rotation, transformComponent.scale);
			return true;
		}

		return false;
	}

	void GUI_Guizmo::setTranslation() {
		m_operation = ImGuizmo::OPERATION::TRANSLATE;
	}

	void GUI_Guizmo::setRotation() {
		m_operation = ImGuizmo::OPERATION::ROTATE;
	}

	void GUI_Guizmo::setScale() {
		m_operation = ImGuizmo::OPERATION::SCALE;
	}

}
