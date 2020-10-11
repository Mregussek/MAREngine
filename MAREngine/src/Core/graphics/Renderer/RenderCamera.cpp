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


#include "RenderCamera.h"
#include "../../ecs/Components/DefaultComponents.h"
#include "../../ecs/Components/CameraComponents.h"


namespace mar::graphics {


	void RenderCamera::calculateCameraTransforms(const ecs::TransformComponent& transform, const ecs::CameraComponent& camera) {
		const float x = trig::cosine(trig::toRadians(transform.angles.y)) * trig::cosine(trig::toRadians(transform.angles.x));
		const float y = trig::sine(trig::toRadians(transform.angles.x));
		const float z = trig::sine(trig::toRadians(transform.angles.y)) * trig::cosine(trig::toRadians(transform.angles.x));
		const vec3 front{ x, y ,z };

		const vec3 eyeToCenter = transform.center + vec3::normalize(front);

		m_position = transform.center;
		m_model = maths::mat4::translation({ 0.f, 0.f, 0.f });
		m_view = mat4::lookAt(transform.center, eyeToCenter, { 0.f, 1.0f, 0.f });
		m_projection = camera.Perspective ?
				mat4::perspective(trig::toRadians(camera.p_fov), camera.p_aspectRatio, camera.p_near, camera.p_far) :
				mat4::orthographic(camera.o_left, camera.o_right, camera.o_top, camera.o_bottom, camera.o_near, camera.o_far);

		m_mvp = m_projection * m_view * m_model;
	}


}
