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


	/*
		Base class for all Entity Events.
	*/
	class IEntityEvents {
	public:

		/*
			Overrided methods should call create Entity function on Scene and invoke other needed events.
		*/
		virtual void onCreateEntity() const { }

		/*
			Overrided methods should call destroy Entity function on Scene and invoke other needed events.
			Imagine situation, when user wants delete some light and renderable from scene, which are assigned to specific entity. 
			Then he can just destroy it.
		*/
		virtual void onDestroyEntity(const Entity& entity) const { }

		/*
			Overrided methods should invoke all events when entity is selected.
			Imagine situation, when user wants to pick some entity from scene, then this event can be called.
		*/
		virtual void onSelectedEntity(const Entity& entity) const { }

		/*
			Overrided methods should invoke all events when entity is unselected.
			Image situation, when user has a choice, selects some entity and then decides to change selected entity.
			On entity, which is unselected can be called this event.
		*/
		virtual void onUnselectedEntity(const Entity& entity) const { }

		/*
			Overrided methods should call copy Entity function on Scene and invoke other needed events.
			There are some situation, when entity can be copied, there is possibility to create duplicates by this event.
		*/
		virtual void onCopyEntity(const Entity& entity) const { }

		/*
			Overrided methods should call set visible Entity function on Scene and invoke other needed events.
			When Entity has RenderableComponent and some material, then it can be rendered. By calling this function
			we can be sure that entity will be visible.
		*/
		virtual void onSetVisibleEntity(const Entity& entity) const { }

		/*
			Overrided methods should call set invisible Entity function on Scene and invoke other needed events.
			When Entity is already visible and we want it to disappear, then we can call this event. Its job is to
			be sure, that entity won't be visible anymore.
		*/
		virtual void onSetInvisibleEntity(const Entity& entity) const { }

	};


}


#endif // !MAR_ENGINE_I_ENTITY_EVENTS_H
