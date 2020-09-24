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


#include "ScriptingComponents.h"
#include "../../../Engine.h"
#include "../ECSLogs.h"


namespace mar {
	namespace ecs {


		std::string ScriptComponent::changeSlashesToDots(std::string str) {
			str = engine::MAREngine::getEngine()->getAssetsPath() + str;

			size_t pos = str.find("/");

			while (pos != std::string::npos) {
				str.replace(pos, 1, ".");
				pos = str.find("/", pos + 1);
			}

			std::string rtn = str.substr(0, str.size() - 3);

			ECS_TRACE("SCRIPT_COMPONENT: changing slashes {} to dots {}", str, rtn);

			return rtn;
		}

		std::string ScriptComponent::getModuleFromPath(std::string str) {
			str = str.substr(str.find_last_of("/") + 1, str.size());
			std::string rtn = str.substr(0, str.size() - 3);

			ECS_TRACE("SCRIPT_COMPONENT: returning module {} from path {}", str, rtn);

			return rtn;
		}


} }