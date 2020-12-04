/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "GUI_Guizmo.h"
#include "../../Camera/Camera.h"
#include "../../../Core/ecs/Components/DefaultComponents.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/SceneEvents.h"
#include "../../../Window/Window.h"


namespace mar::editor {


	void GUI_Guizmo::selectType() {
		//if (window::Window::isKeyPressed(MAR_KEY_LEFT_CONTROL)) {}

		if (window::Window::isKeyPressed(MAR_KEY_Z)) { setTranslation(); }
		if (window::Window::isKeyPressed(MAR_KEY_X)) { setRotation(); }
		if (window::Window::isKeyPressed(MAR_KEY_C)) { setScale(); }
	}

	void GUI_Guizmo::draw(const Camera& editorCamera, const ecs::Entity& currentEntity) const {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		const auto windowPos = ImGui::GetWindowPos();
		const auto windowSize = ImGui::GetWindowSize();
		ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

		auto& transform = currentEntity.getComponent<ecs::TransformComponent>();

		const graphics::RenderCamera* renderCam = editorCamera.getCameraData();
		const float* viewPtr = renderCam->getView().value_ptr();
		const float* projPtr = renderCam->getProjection().value_ptr();
		float* transfromPtr = transform.transform.value_ptr_nonconst();
		ImGuizmo::Manipulate(viewPtr, projPtr, m_operation, ImGuizmo::MODE::LOCAL, transfromPtr);

		if (ImGuizmo::IsUsing()) {
			maths::vec3 pos, rot, sca;
			maths::mat4::decompose(transform.transform, pos, rot, sca);

			transform.center = pos;
			transform.angles += (rot - transform.angles);
			transform.scale = sca;

			ecs::SceneEvents::Instance().onTransformUpdate(currentEntity);
		}
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
