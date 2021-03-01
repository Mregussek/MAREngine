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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/mat4x4.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>


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

	void decomposeLocal(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}
	}

	bool GUI_Guizmo::draw(const Camera& editorCamera, TransformComponent& transformComponent) const {
		using namespace maths;

		glm::vec3 pos{
			transformComponent.position.x,
			transformComponent.position.y,
			transformComponent.position.z
		};
		glm::vec3 rot{
			transformComponent.rotation.x,
			transformComponent.rotation.y,
			transformComponent.rotation.z
		};
		glm::vec3 scal{
			transformComponent.scale.x,
			transformComponent.scale.y,
			transformComponent.scale.z
		};
		glm::mat4 tran{
			glm::translate(glm::mat4(1.f), pos)
			* glm::toMat4(glm::quat(rot))
			* glm::scale(glm::mat4(1.f), scal)
		};

		mat4 transform{ transformComponent.getTransform() };
		//float* pTransform{ transform.value_ptr_nonconst() };
		float* pTransform{ glm::value_ptr(tran) };

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
			decomposeLocal(tran, pos, rot, scal);
			transformComponent.position = {
				pos.x, pos.y, pos.z
			};
			transformComponent.rotation = {
				rot.x, rot.y, rot.z
			};
			transformComponent.scale = {
				scal.x, scal.y, scal.z
			};

			
			//vec4 quaternion;
 			//mat4::decompose(transform,
			//	transformComponent.position, quaternion, transformComponent.scale
			//);
			//transformComponent.rotation = maths::quat::quatToEulerAngles(quaternion);
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
