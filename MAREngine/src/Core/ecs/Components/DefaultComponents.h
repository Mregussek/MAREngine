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


#ifndef MAR_ENGINE_ECS_COMPONENTS_DEFAULT_COMPONENTS_H
#define MAR_ENGINE_ECS_COMPONENTS_DEFAULT_COMPONENTS_H


#include "../../../mar.h"
#include "../../graphics/Mesh/Vertex.h"


namespace marengine {


	struct TagComponent {
		std::string tag{ "empty" };

		TagComponent() = default;
		TagComponent(const TagComponent& id) = default;
		TagComponent(std::string t)
			: tag(std::move(t))
		{}

		operator std::string& () { return tag; }
		operator const std::string& () const { return tag; }
	};

	struct RenderableComponent {
		std::string name{ "empty" };
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		RenderableComponent() = default;
		RenderableComponent(const RenderableComponent& ren) = default;
		RenderableComponent(std::string i)
			: name(std::move(i))
		{}
		RenderableComponent(std::string i, const std::vector<Vertex>& ver, const std::vector<uint32_t>& ind)
			: name(std::move(i)),
			vertices(ver),
			indices(ind)
		{}

		operator const std::string& () const { return name; }
		operator const std::vector<Vertex>& () const { return vertices; }
		operator const std::vector<uint32_t>& () const { return indices; }
	};

	struct CollectionRenderableComponent {
		std::string id{ "empty" };
		uint32_t entitiesRenderableCount{ 0 };

		CollectionRenderableComponent() = default;
		CollectionRenderableComponent(const CollectionRenderableComponent& crc) = default;
		CollectionRenderableComponent(std::string str, uint32_t count) : 
			id(std::move(str)),
			entitiesRenderableCount(count)
		{}

		operator std::string& () { return id; }
		operator const std::string& () const { return id; }
	};

	struct TransformComponent {
		maths::vec3 center{ 0.f, 0.f, 0.f };
		maths::vec3 angles{ 0.f, 0.f, 0.f };
		maths::vec3 scale{ 1.f, 1.f, 1.f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& tc) = default;
		TransformComponent(maths::vec3 newCenter, maths::vec3 newAngles, maths::vec3 newScale) :
			center(std::move(newCenter)),
			angles(std::move(newAngles)),
			scale(std::move(newScale))
		{}

		maths::mat4 getTransform() const;
	};


}


#endif // !MAR_ENGINE_ECS_COMPONENTS_DEFAULT_COMPONENTS_H
