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


#ifndef MAR_ENGINE_T_EVENTS_COMPONENT_ENTITY_H
#define MAR_ENGINE_T_EVENTS_COMPONENT_ENTITY_H


#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	class FEventsComponentEntity {
	public:

		static FEventsComponentEntity* Instance;


		template<typename T> void onAdd(const Entity& entity) { 
			entity.template addComponent<T>();
		}

		template<typename T> void onUpdate(const Entity& entity) { 
		
		}

		template<typename T> void onRemove(const Entity& entity) { 
			entity.template removeComponent<T>();
		}

	};

	// Here add declarations for specific component event implementations ...

	template<> void FEventsComponentEntity::onAdd<ColorComponent>(const Entity& entity);

}


#endif // !MAR_ENGINE_T_COMPONENT_WIDGET_EVENTS_H
