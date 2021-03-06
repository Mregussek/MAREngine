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


#ifndef MAR_ENGINE_DEFAULT_COMPONENTS_H
#define MAR_ENGINE_DEFAULT_COMPONENTS_H


#include "../../../mar.h"
#include "../../graphics/Mesh/Vertex.h"


namespace marengine {


	/*
		TagComponent - some unique name for entity, that can be more readable to user.
	*/
	struct TagComponent {

		std::string tag{ "empty" };

		TagComponent() = default;
		TagComponent(const TagComponent& id) = default;
		TagComponent(std::string t);

	};

	/*
		RenderableComponent - contains vertices and indices, that can be pushed to batches.
		RenderableComponent::name is a unique name for renderable that component contains. For instance,
		for default MAREngine shapes it can have value "Cube", "Surface", but for loaded external files 
		it should have path to that file.
	*/
	struct RenderableComponent {

		std::string name{ "empty" };
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		RenderableComponent() = default;
		RenderableComponent(const RenderableComponent& ren) = default;
		RenderableComponent(std::string i);
		RenderableComponent(std::string i, const std::vector<Vertex>& ver, const std::vector<uint32_t>& ind);

	};

	/*
		TransformComponent - contains information about current position, rotation and scale of entity.
		Returns maths::mat4 transform by getTransform method.

		TransformComponent is used by batches, position member value is also used by LightComponents.
	*/
	struct TransformComponent {

		maths::vec3 position{ 0.f, 0.f, 0.f };
		maths::vec3 rotation{ 0.f, 0.f, 0.f };
		maths::vec3 scale{ 1.f, 1.f, 1.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& tc) = default;
		TransformComponent(maths::vec3 newCenter, maths::vec3 newAngles, maths::vec3 newScale);

		/*
			Method returns maths::mat4 transform recomposed with current member values.
		*/
		maths::mat4 getTransform() const;

	};


}


#endif // !MAR_ENGINE_ECS_COMPONENTS_DEFAULT_COMPONENTS_H
