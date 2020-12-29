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


#include "GuizmoWidget.h"
#include "../../Camera/Camera.h"
#include "../../../Core/ecs/Components/DefaultComponents.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/events/SceneEvents.h"
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
		if (draw(editorCamera, transform)) {
			SceneEvents::Instance().onTransformUpdate(currentEntity);
		}
	}

	void GUI_Guizmo::draw(const Camera& editorCamera, const EntityCollection& currentCollection) const {
		auto& transform = currentCollection.getComponent<TransformComponent>();
		if (draw(editorCamera, transform)) {
			SceneEvents::Instance().onCollectionTransformUpdate(&currentCollection, transform);
		}
	}

	bool GUI_Guizmo::draw(const Camera& editorCamera, TransformComponent& transform) const {
		using namespace maths;

		mat4 matrix{ transform.getTransform() };

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		const auto windowPos = ImGui::GetWindowPos();
		const auto windowSize = ImGui::GetWindowSize();
		ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

		const RenderCamera* renderCam = editorCamera.getCameraData();
		const float* viewPtr = renderCam->getView().value_ptr();
		const float* projPtr = renderCam->getProjection().value_ptr();
		float* transfromPtr = matrix.value_ptr_nonconst();
		ImGuizmo::Manipulate(viewPtr, projPtr, m_operation, ImGuizmo::MODE::LOCAL, transfromPtr);

		if (ImGuizmo::IsUsing()) {
			vec3 rot;
			mat4::decompose(matrix, transform.center, rot, transform.scale);
			transform.angles = vec3(trig::toDegrees(rot.x), trig::toDegrees(rot.y), trig::toDegrees(rot.z));

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
