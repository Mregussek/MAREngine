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


#ifndef MAR_ENGINE_F_EVENTS_ENTITY_WIDGET_H
#define MAR_ENGINE_F_EVENTS_ENTITY_WIDGET_H


#include "../../Core/ecs/Entity/Entity.h"


namespace marengine {


	class FEventsEntityWidget {
	public:

		static void onCreateEntity();
		static void onDestroyEntity(const Entity& entity);

		static void onSelectedEntity(const Entity& entity);
		static void onUnselectedEntity(const Entity& entity);

		static void onCopyEntity(const Entity& entity);

		static void onSetVisibleEntity(const Entity& entity);
		static void onSetInvisibleEntity(const Entity& entity);

	};


}


#endif // !MAR_ENGINE_F_ENTITY_WIDGET_EVENTS_H
