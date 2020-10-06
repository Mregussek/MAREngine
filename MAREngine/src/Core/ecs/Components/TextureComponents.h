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


#ifndef MAR_ENGINE_ECS_COMPONENTS_TEXTURE_COMPONENTS_H
#define MAR_ENGINE_ECS_COMPONENTS_TEXTURE_COMPONENTS_H


#include "../../../mar.h"


namespace mar {
	namespace ecs {


		struct ColorComponent {
			maths::vec3 texture{ 0.5f, 0.5f, 0.5f };

			ColorComponent() = default;
			ColorComponent(const ColorComponent& cc) = default;
			ColorComponent(const maths::vec3& col)
				: texture(col)
			{}

			operator maths::vec3& () { return texture; }
			operator const maths::vec3& () const { return texture; }
		};

		struct Texture2DComponent {
			std::string texture{ "empty" };

			Texture2DComponent() = default;
			Texture2DComponent(const Texture2DComponent& tex) = default;
			Texture2DComponent(const std::string& tex)
				: texture(tex)
			{}

			operator std::string& () { return texture; }
			operator const std::string& () const { return texture; }
		};

		struct TextureCubemapComponent {
			std::string texture{ "empty" };

			TextureCubemapComponent() = default;
			TextureCubemapComponent(const TextureCubemapComponent& cub) = default;
			TextureCubemapComponent(const std::string& cub)
				: texture(cub)
			{}

			operator std::string& () { return texture; }
			operator const std::string& () const { return texture; }
		};


} }


#endif // !MAR_ENGINE_ECS_COMPONENTS_TEXTURE_COMPONENTS_H