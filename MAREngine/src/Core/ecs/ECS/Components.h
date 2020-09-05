/**
 *				MAREngine - open source 3D game engine
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


#ifndef MAR_ENGINE_ECS_COMPONENTS_ENTITY_H
#define MAR_ENGINE_ECS_COMPONENTS_ENTITY_H


#include "../../../mar.h"
#include "../../scripting/PythonScript.h"


namespace mar {
	namespace ecs {


		struct TagComponent {
			std::string tag{ "empty" };

			TagComponent() = default;
			TagComponent(const TagComponent& id) = default;
			TagComponent(std::string t)
				: tag(t)
			{}

			operator std::string&() { return tag; }
			operator const std::string&() const { return tag; }
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
			operator const std::vector<float>&() const { return vertices; }
			operator const std::vector<uint32_t>&() const { return indices; }
		};

		struct TransformComponent {
			float general_scale{ 1.f };
			maths::vec3 center{ 0.f, 0.f, 0.f };
			maths::vec3 angles{ 0.f, 0.f, 0.f };
			maths::vec3 scale{1.f, 1.f, 1.f };

			maths::mat4 transform{ 1.f };

			TransformComponent() = default;
			TransformComponent(const TransformComponent& tc) = default;

			operator maths::mat4&() { return transform; }
			operator const maths::mat4&() const { return transform; }
		};

		struct ColorComponent {
			maths::vec3 texture{ 0.5f, 0.5f, 0.5f };

			ColorComponent() = default;
			ColorComponent(const ColorComponent& cc) = default;
			ColorComponent(const maths::vec3& col)
				: texture(col)
			{}

			operator maths::vec3&() { return texture; }
			operator const maths::vec3&() const { return texture; }
		};

		struct Texture2DComponent {
			std::string texture{ "empty" };

			Texture2DComponent() = default;
			Texture2DComponent(const Texture2DComponent& tex) = default;
			Texture2DComponent(const std::string& tex)
				: texture(tex)
			{}

			operator std::string&() { return texture; }
			operator const std::string&() const { return texture; }
		};

		struct TextureCubemapComponent {
			std::string texture{ "empty" };

			TextureCubemapComponent() = default;
			TextureCubemapComponent(const TextureCubemapComponent& cub) = default;
			TextureCubemapComponent(const std::string& cub)
				: texture(cub)
			{}

			operator std::string&() { return texture; }
			operator const std::string&() const { return texture; }
		};

		struct CameraComponent {
			std::string id{ "secondary" };

			bool Perspective{ true }; // true - perspective | false - orthographic

			float p_fov{ 45.f };
			float p_aspectRatio{ 4.f / 3.f };
			float p_near{ 0.01f };
			float p_far{ 100.0f };
			
			float o_left{ -10.f };
			float o_right{ 10.f };
			float o_top{ 10.f };
			float o_bottom{ -10.f };
			float o_near{ 0.01f };
			float o_far{ 100.0f };

			CameraComponent() = default;
			CameraComponent(const CameraComponent& cam) = default;
		};

		struct LightComponent {
			maths::vec3 ambient{ 0.5f, 0.5f, 0.5f };
			maths::vec3 diffuse{ 0.9f, 0.9f, 0.9f };
			maths::vec3 specular{ 0.5f, 0.5f, 0.5f };

			float constant{ 1.0f };
			float linear{ 0.045f };
			float quadratic{ 0.0075f };

			float shininess{ 64.0f };

			LightComponent() = default;
			LightComponent(const LightComponent& li) = default;
		};

		struct ScriptComponent {
			std::string script{ "empty" };
			std::string source{ "empty" };
			scripting::PythonScript ps;

			ScriptComponent() = default;
			ScriptComponent(const ScriptComponent& sc) = default;
			ScriptComponent(std::string s)
				: script(s)
			{}

			operator std::string& () { return script; }
			operator const std::string& () const { return script; }
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


#endif // !MAR_ENGINE_ECS_COMPONENTS_ENTITY_H