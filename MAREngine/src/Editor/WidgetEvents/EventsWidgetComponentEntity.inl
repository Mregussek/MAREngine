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


#ifndef MAR_ENGINE_T_EVENTS_WIDGET_COMPONENT_ENTITY_INL
#define MAR_ENGINE_T_EVENTS_WIDGET_COMPONENT_ENTITY_INL


#include "EventsWidgetComponentEntity.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Components/Components.h"
#include "../../Core/events/SceneEvents.h"


namespace marengine {


	template<typename T> void TEventsWidgetComponentEntity::onAdd(const Entity& entity) {
		const auto& component{ entity.template addComponent<T>() };
	}

	template<typename T> void TEventsWidgetComponentEntity::onUpdate(const Entity& entity) {

	}

	template<typename T> void TEventsWidgetComponentEntity::onRemove(const Entity& entity) {
		entity.template removeComponent<T>();
	}

	template<> void TEventsWidgetComponentEntity::onAdd<ColorComponent>(const Entity& entity) {
		entity.addComponent<ColorComponent>();
		SceneEvents::Instance().onColorAdd();
	}


}


#endif // !MAR_ENGINE_T_EVENTS_WIDGET_COMPONENT_ENTITY_INL
