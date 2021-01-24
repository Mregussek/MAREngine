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


#ifndef MAR_ENGINE_TEXTURE_COMPONENTS_H
#define MAR_ENGINE_TEXTURE_COMPONENTS_H


#include "../../../mar.h"


namespace marengine {


	/*
		ColorComponent - contains maths::vec4 color instance, that describes entity with
		some color. vec4::x stands for R, y for G, z for B and w for A.
	*/
	struct ColorComponent {

		maths::vec4 color{ 0.5f, 0.5f, 0.5f, 1.f };

		ColorComponent() = default;
		ColorComponent(const ColorComponent& cc) = default;
		ColorComponent(maths::vec4 c);

	};

	/*
		Texture2DComponent - contains texturePath string instance, that describes entity
		with some texture2D. texturePath should have end with .jpg extension.
	*/
	struct Texture2DComponent {

		std::string texturePath{ "empty" };

		Texture2DComponent() = default;
		Texture2DComponent(const Texture2DComponent& tex) = default;
		Texture2DComponent(std::string tex);

	};

	/*
		TextureCubemapComponent - should not be used for now.
	*/
	struct TextureCubemapComponent {

		std::string texturePath{ "empty" };

		TextureCubemapComponent() = default;
		TextureCubemapComponent(const TextureCubemapComponent& cub) = default;
		TextureCubemapComponent(std::string cub);

	};


}


#endif // !MAR_ENGINE_ECS_COMPONENTS_TEXTURE_COMPONENTS_H
