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


#ifndef MAR_ENGINE_I_ENTITY_EVENTS_H
#define MAR_ENGINE_I_ENTITY_EVENTS_H


namespace marengine {

	class Entity;


	class IEntityEvents {
	public:

		virtual void onCreateEntity() const { }
		virtual void onDestroyEntity(const Entity& entity) const { }

		virtual void onSelectedEntity(const Entity& entity) const { }
		virtual void onUnselectedEntity(const Entity& entity) const { }

		virtual void onCopyEntity(const Entity& entity) const { }

		virtual void onSetVisibleEntity(const Entity& entity) const { }
		virtual void onSetInvisibleEntity(const Entity& entity) const { }

	};



}


#endif // !MAR_ENGINE_I_ENTITY_EVENTS_H
