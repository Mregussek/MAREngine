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


#ifndef MAR_ENGINE_ECS_SYSTEMS_H
#define MAR_ENGINE_ECS_SYSTEMS_H


#include "../../../mar.h"


namespace mar {
	namespace ecs {

		struct TransformComponent;


		class System {
		public:

			static void handleTransformComponent(TransformComponent& tran);

			static maths::mat4 handleTransformComponent(maths::vec3& center, maths::vec3& angles, maths::vec3& scale);

			static std::string changeSlashesToDots(std::string str);

			static std::string getModuleFromPath(std::string str);

		};


} }

#endif // !MAR_ENGINE_ECS_SYSTEMS_H
