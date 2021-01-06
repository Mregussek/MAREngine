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


#ifndef MAR_ENGINE_LIGHT_DEFINITIONS_H
#define MAR_ENGINE_LIGHT_DEFINITIONS_H


#include "../../../mar.h"


namespace marengine {


	struct FPointLight {

		maths::vec4 position{ 0.f, 0.f, 0.f, 1.f };
		maths::vec4 ambient{ 0.5f, 0.5f, 0.5f, 1.f };
		maths::vec4 diffuse{ 0.9f, 0.9f, 0.9f, 1.f };
		maths::vec4 specular{ 0.5f, 0.5f, 0.5f, 1.f };

		float constant{ 1.0f };
		float linear{ 0.045f };
		float quadratic{ 0.0075f };
		float shininess{ 64.0f };

	};


	typedef std::vector<FPointLight> FPointLightsArray;


}


#endif // !MAR_ENGINE_LIGHT_DEFINITIONS_H
