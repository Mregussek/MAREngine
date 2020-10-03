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


#include "mar_includes.h"
#include "mar_logging.h"

/*!
	mar is a main namespace for MAREngine
	If class is here declared, it means that there is linker problem for other files with it,
	so I decided to put it here.
*/
namespace mar {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

#ifndef MAR_ENGINE_SETTINGS
#define MAR_ENGINE_SETTINGS

	namespace settings {
		inline static uint32_t maxVerticesCount = 500000;
		inline static uint32_t maxIndicesCount = 500000 / 2;
	}

	template<typename T>
	struct PointerFactory {
		inline static uint32_t ref_counter;
		inline static uint32_t scope_counter;

		template<typename... Args>
		static std::unique_ptr<T> makeScope(Args&&... args) {
			ref_counter++;
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename... Args>
		static std::shared_ptr<T> makeRef(Args&&... args) {
			scope_counter++;
			return std::make_shared<T>(std::forward<Args>(args)...);
		}
	};

#endif // !MAR_ENGINE_SETTINGS
}