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


#include "RenderCamera.h"
#include "../../ecs/Components/DefaultComponents.h"
#include "../../ecs/Components/CameraComponents.h"


namespace marengine {


	void RenderCamera::calculateCameraTransforms(const TransformComponent& transform, const CameraComponent& camera) {
		const float xRad{ trig::toRadians(transform.rotation.x) };
		const float yRad{ trig::toRadians(transform.rotation.y) };
		
		const vec3 front{ 
			trig::cosine(yRad) * trig::cosine(xRad), 
			trig::sine(xRad),
			trig::sine(yRad) * trig::cosine(xRad) 
		};
		const vec3 eyeToCenter{ transform.position + vec3::normalize(front) };

		calculateModel({ 0.f, 0.f, 0.f });
		calculateView(transform.position, eyeToCenter, { 0.f, 1.0f, 0.f });

		if (camera.Perspective) { calculatePerspective(camera.p_fov, camera.p_aspectRatio, camera.p_near, camera.p_far); }
		else { calculateOrthographic(camera.o_left, camera.o_right, camera.o_top, camera.o_bottom, camera.o_near, camera.o_far); }

		recalculateMVP();
	}

	void RenderCamera::calculatePerspective(float zoom, float aspectRatio, float nearPlane, float farPlane) {
		m_projection = mat4::perspective(trig::toRadians(zoom), aspectRatio, nearPlane, farPlane);
	}

	void RenderCamera::calculateOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane) {
		m_projection = mat4::orthographic(left, right, top, bottom, nearPlane, farPlane);
	}

	void RenderCamera::calculateView(maths::vec3 position, maths::vec3 lookAt, maths::vec3 up) {
		m_position = position;
		m_view = mat4::lookAt(position, lookAt, up);
	}

	void RenderCamera::calculateModel(maths::vec3 arg) {
		m_model = mat4::translation(arg);
	}

	void RenderCamera::recalculateMVP() {
		m_mvp = m_projection * m_view;
	}

	const maths::mat4& RenderCamera::getProjection() const { 
		return m_projection; 
	}

	const maths::mat4& RenderCamera::getView() const { 
		return m_view; 
	}

	const maths::mat4& RenderCamera::getModel() const { 
		return m_model; 
	}

	const maths::mat4& RenderCamera::getMVP() const {
		return m_mvp;
	}

	const maths::vec3& RenderCamera::getPosition() const { 
		return m_position; 
	}


}
