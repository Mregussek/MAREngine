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


#include "EventsWidgetComponentCollection.h"
#include "../../Core/ecs/Entity/EntityCollection.h"


namespace marengine {


	template<typename T> void TEventsWidgetComponentCollection<T>::onAdd(const EntityCollection& collection) {
		const auto& component{ collection.template addComponent<T>() };
	}

	template<typename T> void TEventsWidgetComponentCollection<T>::onUpdate(const EntityCollection& collection) {
		collection.template removeComponent<T>();
	}

	template<typename T> void TEventsWidgetComponentCollection<T>::onRemove(const EntityCollection& collection) {

	}


}
