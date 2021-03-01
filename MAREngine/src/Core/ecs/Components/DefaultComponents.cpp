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


#include "DefaultComponents.h"
#include "../ECSLogs.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/mat4x4.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace marengine {


	TagComponent::TagComponent(std::string t) :
		tag(std::move(t))
	{}

	RenderableComponent::RenderableComponent(std::string i) : 
		name(std::move(i))
	{}
	RenderableComponent::RenderableComponent(std::string i, const std::vector<Vertex>& ver, const std::vector<uint32_t>& ind) :
		name(std::move(i)),
		vertices(ver),
		indices(ind)
	{}

	TransformComponent::TransformComponent(maths::vec3 newCenter, maths::vec3 newAngles, maths::vec3 newScale) :
		position(newCenter),
		rotation(newAngles),
		scale(newScale)
	{}

	maths::mat4 TransformComponent::getTransform() const {
		maths::mat4 tran;
		//maths::mat4::recompose(tran, position, rotation, scale);
		//return tran;

		glm::vec3 pos{
			position.x,
			position.y,
			position.z
		};
		glm::vec3 rot{
			rotation.x,
			rotation.y,
			rotation.z
		};
		glm::vec3 scal{
			scale.x,
			scale.y,
			scale.z
		};
		glm::mat4 glmTran{
			glm::translate(glm::mat4(1.f), pos)
			* glm::toMat4(glm::quat(rot))
			* glm::scale(glm::mat4(1.f), scal)
		};
		float* pGlmTran{ glm::value_ptr(glmTran) };
		for (size_t i = 0; i < 16; i++) {
			tran.elements[i] = pGlmTran[i];
		}

		return tran;

	}


}
