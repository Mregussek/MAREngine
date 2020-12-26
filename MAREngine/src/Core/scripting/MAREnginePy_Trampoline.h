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


#ifndef MAR_ENGINE_SCRIPTING_ENGINE_TRAMPOLINE_H
#define MAR_ENGINE_SCRIPTING_ENGINE_TRAMPOLINE_H


#include "../../mar.h"
#include "../ecs/Components/Components.h"
#include "../ecs/Entity/Entity.h"


namespace marengine {


	class PyEntity {
	public:
		TransformComponent transform;
		LightComponent light;
		CameraComponent camera;
		ColorComponent color;

		virtual void start() { }
		virtual void update() { }
	};

	class PyTrampoline : public PyEntity {
	public:
		using PyEntity::PyEntity;

		void start() override {
			PYBIND11_OVERLOAD(
				void,
				PyEntity,
				start
			);
		}

		void update() override {
			PYBIND11_OVERLOAD(
				void,
				PyEntity,
				update
			);
		}
	};


}


#endif // !MAR_ENGINE_SCRIPTING_ENGINE_EMBED_HELPERS_H
