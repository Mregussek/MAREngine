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


#include "DefaultComponents.h"
#include "../ECSLogs.h"


namespace mar::ecs {


	void TransformComponent::recalculate() {
		using namespace maths;

		transform =
			mat4::translation(center) *
			mat4::rotation(trig::toRadians(angles.x), { 1.f, 0.f, 0.f }) *
			mat4::rotation(trig::toRadians(angles.y), { 0.f, 1.f, 0.f }) *
			mat4::rotation(trig::toRadians(angles.z), { 0.f, 0.f, 1.f }) *
			mat4::scale(scale);

		ECS_TRACE("TRANSFORM_COMPONENT: calculated new TransformComponent!");
	}

	maths::mat4 TransformComponent::calculate(const maths::vec3& center, const maths::vec3& angles, const maths::vec3& scale) {
		using namespace maths;

		return	mat4::translation(center) *
			mat4::rotation(trig::toRadians(angles.x), { 1.f, 0.f, 0.f }) *
			mat4::rotation(trig::toRadians(angles.y), { 0.f, 1.f, 0.f }) *
			mat4::rotation(trig::toRadians(angles.z), { 0.f, 0.f, 1.f }) *
			mat4::scale(scale);

		ECS_TRACE("TRANSFORM_COMPONENT: calculated new TransformComponent (static call)!");
	}


}
