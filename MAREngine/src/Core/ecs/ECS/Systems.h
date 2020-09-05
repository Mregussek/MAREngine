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
#include "../ECSLogs.h"
#include "Components.h"


namespace mar {
	namespace ecs {


		class System {
		public:

			static void handleTransformComponent(TransformComponent& tran) {
				using namespace maths;

				tran.transform =
					mat4::translation(tran.center) *
					mat4::rotation(Trig::toRadians(tran.angles.x), { 1.f, 0.f, 0.f }) *
					mat4::rotation(Trig::toRadians(tran.angles.y), { 0.f, 1.f, 0.f }) *
					mat4::rotation(Trig::toRadians(tran.angles.z), { 0.f, 0.f, 1.f }) *
					mat4::scale(tran.scale);

				ECS_TRACE("SYSTEM: calculated new TransformComponent!");
			}

			static maths::mat4 handleTransformComponent(maths::vec3& center, maths::vec3& angles, maths::vec3& scale) {
				using namespace maths;

				return	mat4::translation(center) *
						mat4::rotation(Trig::toRadians(angles.x), { 1.f, 0.f, 0.f }) *
						mat4::rotation(Trig::toRadians(angles.y), { 0.f, 1.f, 0.f }) *
						mat4::rotation(Trig::toRadians(angles.z), { 0.f, 0.f, 1.f }) *
						mat4::scale(scale);

				ECS_TRACE("SYSTEM: calculated new TransformComponent!");
			}

			static std::string changeSlashesToDots(std::string str) {
				size_t pos = str.find("/");

				while (pos != std::string::npos) {
					str.replace(pos, 1, ".");
					pos = str.find("/", pos + 1);
				}

				return str.substr(0, str.size() - 3);
			}

			static std::string getModuleFromPath(std::string str) {
				str = str.substr(str.find_last_of("/") + 1, str.size());
				return str.substr(0, str.size() - 3);
			}

		};


} }

#endif // !MAR_ENGINE_ECS_SYSTEMS_H
