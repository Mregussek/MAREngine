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


#ifndef MAR_ENGINE_ECS_ENGINE_ONLY_COMPONENTS_H
#define MAR_ENGINE_ECS_ENGINE_ONLY_COMPONENTS_H


#include "../../../mar.h"


namespace mar {
	namespace ecs {


		enum class EntityComponents {
			COLOR,					// 0
			TEXTURE2D,				// 1
			CUBEMAP,				// 2
			RENDERABLE,				// 3
			TAG,					// 4
			TRANSFORM,				// 5
			LIGHT,					// 6
			CAMERA,					// 7
			SCRIPT					// 8
		};

		inline static const std::vector<std::pair<EntityComponents, const char*>> AllExistingComponents{
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

		struct RenderPipelineComponent {
			size_t container_index{ 0 };
			size_t transform_index{ 0 };
			size_t color_index{ 0 };
			size_t container_light_index{ 0 };
			size_t light_index{ 0 };

			RenderPipelineComponent() = default;
			RenderPipelineComponent(const RenderPipelineComponent& rpc)
				: container_index(rpc.container_index),
				transform_index(rpc.transform_index),
				color_index(rpc.color_index),
				container_light_index(rpc.container_light_index),
				light_index(rpc.light_index)
			{}
		};


} }


#define ECS_RENDERABLE	::mar::ecs::EntityComponents::RENDERABLE
#define ECS_COLOR		::mar::ecs::EntityComponents::COLOR
#define ECS_TEXTURE2D	::mar::ecs::EntityComponents::TEXTURE2D
#define ECS_CUBEMAP		::mar::ecs::EntityComponents::CUBEMAP
#define ECS_TAG			::mar::ecs::EntityComponents::TAG
#define ECS_TRANSFORM	::mar::ecs::EntityComponents::TRANSFORM
#define ECS_LIGHT		::mar::ecs::EntityComponents::LIGHT
#define ECS_CAMERA		::mar::ecs::EntityComponents::CAMERA
#define ECS_SCRIPT		::mar::ecs::EntityComponents::SCRIPT


#endif // !MAR_ENGINE_ECS_ENGINE_ONLY_COMPONENTS_H
