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
		virtual void onCreateEntity() const = 0;

		/*
			Overrided methods should call destroy Entity function on Scene and invoke other needed events.
			Imagine situation, when user wants delete some light and renderable from scene, which are assigned to specific entity. 
			Then he can just destroy it.
		*/
		virtual void onDestroyEntity(const Entity& entity) const = 0;

		/*
			Overrided methods should invoke all events when entity is selected.
			Imagine situation, when user wants to pick some entity from scene, then this event can be called.
		*/
		virtual void onSelectedEntity(const Entity& entity) const = 0;

		/*
			Overrided methods should invoke all events when entity is unselected.
			Image situation, when user has a choice, selects some entity and then decides to change selected entity.
			On entity, which is unselected can be called this event.
		*/
		virtual void onUnselectedEntity(const Entity& entity) const = 0;

		/*
			Overrided methods should call copy Entity function on Scene and invoke other needed events.
			There are some situation, when entity can be copied, there is possibility to create duplicates by this event.
		*/
		virtual void onCopyEntity(const Entity& entity) const = 0;

		/*
			Overrided methods should call set visible Entity function on Scene and invoke other needed events.
			When Entity has RenderableComponent and some material, then it can be rendered. By calling this function
			we can be sure that entity will be visible.
		*/
		virtual void onSetVisibleEntity(const Entity& entity) const = 0;

		/*
			Overrided methods should call set invisible Entity function on Scene and invoke other needed events.
			When Entity is already visible and we want it to disappear, then we can call this event. Its job is to
			be sure, that entity won't be visible anymore.
		*/
		virtual void onSetInvisibleEntity(const Entity& entity) const = 0;

		/*
			Overrided methods should call assign child to entity Entity function and invoke other needed events.
			When we want some entities to be relative to other, we need to make parents and childs.
			When a child is created, we should use this method.
		*/
		virtual void onAssignChild(const Entity& entity, const Entity& child) const = 0;

		/*
			Overrided methods should call remove child from entity Entity function and invoke other needed events.
			There can be situation, when child is assigned to some entity and everything is fine. But sometimes we want
			to remove child from entity, for instance assign to other entity. Then we can call this event.
		*/
		virtual void onRemoveChild(const Entity& entity, const Entity& child) const = 0;

		/*
			Overrided methods should call create child Entity function when child is created. Then automatically 
			child is assigned to given entity.
		*/
		virtual void onCreateChild(const Entity& entity) const = 0;

		/*
			Overrided methods should call destroy child Entity function when child is destroyed. Then automatically
			child is removed from given entity.
		*/
		virtual void onDestroyChild(const Entity& entity, const Entity& child) const = 0;

	};


}


#endif // !MAR_ENGINE_I_ENTITY_EVENTS_H
