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
		tran.recompose(position, maths::quat(rotation), scale);
		return tran;
	}


}
