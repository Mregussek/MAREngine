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


namespace mar::editor {


	void GUI_Guizmo::draw(const Camera& editorCamera, const ecs::Entity& currentEntity) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		const auto windowPos = ImGui::GetWindowPos();
		const auto windowSize = ImGui::GetWindowSize();
		ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

		auto& transform = currentEntity.getComponent<ecs::TransformComponent>();

		const graphics::RenderCamera* renderCam = editorCamera.getCameraData();
		const float* viewPtr = maths::mat4::value_ptr(renderCam->getView());
		const float* projPtr = maths::mat4::value_ptr(renderCam->getProjection());
		float* transfromPtr = maths::mat4::value_ptr_nonconst(transform.transform);
		ImGuizmo::Manipulate(viewPtr, projPtr, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, transfromPtr);
	}

}
