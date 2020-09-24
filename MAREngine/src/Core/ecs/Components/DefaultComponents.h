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


namespace mar {
	namespace ecs {


		struct TagComponent {
			std::string tag{ "empty" };

			TagComponent() = default;
			TagComponent(const TagComponent& id) = default;
			TagComponent(std::string t)
				: tag(t)
			{}

			operator std::string& () { return tag; }
			operator const std::string& () const { return tag; }
		};

		struct RenderableComponent {
			std::string id{ "empty" };
			float shader_id{ 0.f };
			std::vector<float> vertices;
			std::vector<uint32_t> indices;

			RenderableComponent() = default;
			RenderableComponent(const RenderableComponent& ren) = default;
			RenderableComponent(std::string i)
				: id(i)
			{}
			RenderableComponent(std::string i, const std::vector<float>& ver, const std::vector<uint32_t>& ind)
				: id(i),
				vertices(ver),
				indices(ind)
			{}

			operator const std::string& () const { return id; }
			operator const std::vector<float>& () const { return vertices; }
			operator const std::vector<uint32_t>& () const { return indices; }
		};

		struct CollectionRenderableComponent {
			std::string id{ "empty" };

			CollectionRenderableComponent() = default;
			CollectionRenderableComponent(const CollectionRenderableComponent& crc) = default;
			CollectionRenderableComponent(std::string i)
				: id(i)
			{}

			operator std::string& () { return id; }
			operator const std::string& () const { return id; }
		};

		struct TransformComponent {
			float general_scale{ 1.f };
			maths::vec3 center{ 0.f, 0.f, 0.f };
			maths::vec3 angles{ 0.f, 0.f, 0.f };
			maths::vec3 scale{ 1.f, 1.f, 1.f };

			maths::mat4 transform{ 1.f };

			TransformComponent() = default;
			TransformComponent(const TransformComponent& tc) = default;

			operator maths::mat4& () { return transform; }
			operator const maths::mat4& () const { return transform; }

			void recalculate();
			static maths::mat4 calculate(maths::vec3& center, maths::vec3& angles, maths::vec3& scale);
		};

		// ----------------------------------------------------
		// BRIEF OVERVIEW OF ALL COMPONENTS
		// ----------------------------------------------------

		enum class EntityComponents {
			DEFAULT_COMPONENT,		// 0
			COLOR,					// 1
			TEXTURE2D,				// 2
			CUBEMAP,				// 3
			RENDERABLE,				// 4
			TAG,					// 5
			TRANSFORM,				// 6
			LIGHT,					// 7
			CAMERA,					// 8
			SCRIPT					// 9
		};

		inline static const std::vector<std::pair<EntityComponents, const char*>> AllExistingComponents{
				{ EntityComponents::DEFAULT_COMPONENT, "DefaultComponent" },
				{ EntityComponents::COLOR, "ColorComponent" },
				{ EntityComponents::TEXTURE2D, "Texture2DComponent" },
				{ EntityComponents::CUBEMAP, "TextureCubemapComponent" },
				{ EntityComponents::RENDERABLE, "RenderableComponent" },
				{ EntityComponents::TAG, "TagComponent" },
				{ EntityComponents::TRANSFORM, "TransformComponent" },
				{ EntityComponents::LIGHT, "LightComponent" },
				{ EntityComponents::CAMERA, "CameraComponent" },
				{ EntityComponents::SCRIPT, "ScriptComponent" },
		};

		struct Components {
			std::vector<EntityComponents> components;

			Components() = default;
			Components(const Components& com) = default;
			Components(const std::vector<EntityComponents>& vec)
				: components(vec)
			{}

			operator std::vector<EntityComponents>& () { return components; }
			operator const std::vector<EntityComponents>& () const { return components; }
		};

} }


#define ECS_DEFAULT		::mar::ecs::EntityComponents::DEFAULT_COMPONENT
#define ECS_RENDERABLE	::mar::ecs::EntityComponents::RENDERABLE
#define ECS_COLOR		::mar::ecs::EntityComponents::COLOR
#define ECS_TEXTURE2D	::mar::ecs::EntityComponents::TEXTURE2D
#define ECS_CUBEMAP		::mar::ecs::EntityComponents::CUBEMAP
#define ECS_TAG			::mar::ecs::EntityComponents::TAG
#define ECS_TRANSFORM	::mar::ecs::EntityComponents::TRANSFORM
#define ECS_LIGHT		::mar::ecs::EntityComponents::LIGHT
#define ECS_CAMERA		::mar::ecs::EntityComponents::CAMERA
#define ECS_SCRIPT		::mar::ecs::EntityComponents::SCRIPT


#endif // !MAR_ENGINE_ECS_COMPONENTS_DEFAULT_COMPONENTS_H
