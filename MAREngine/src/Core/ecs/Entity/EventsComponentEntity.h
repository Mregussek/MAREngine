/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_F_EVENTS_COMPONENT_ENTITY_H
#define MAR_ENGINE_F_EVENTS_COMPONENT_ENTITY_H


namespace marengine {

	class Entity;
    class FSceneManagerEditor;

	
	/**
	 * @class FEventsComponentEntity EventsComponentEntity.h "Core/ecs/Entity/EventsComponentEntity.h"
	 * @brief FEventsComponentEntity is event class for entity components. With this we can define
	 * events for specific component action. Newly defined components should create own templated methods.
	 */
	class FEventsComponentEntity {
	public:

	    static void create(FSceneManagerEditor* pSceneManagerEditor);

		/**
		 * @brief Event called everytime, when TComponent is added to entity. Remember, method actually adds component!
		 * Also specific implementions should inform other instances, that are looking for those events.
		 * @tparam TComponent structure type of component
		 * @param entity entity, at which add component event is called
		 */
		template<typename TComponent> static void onAdd(const Entity& entity);

		/**
		 * @brief  Event called everytime, when TComponent's parameters are updated, and other instances should know it!
		 * @tparam TComponent structure type of component
		 * @param entity entity, at which update component event is called
		 */
		template<typename TComponent> static void onUpdate(const Entity& entity);

		/**
		 * @brief Event called everytime, when TComponent is removed from entity. Remember, method actually removes component!
		 * Specific implementations should inform other instances, that are looking for those events.
		 * @tparam TComponent structure type of component
		 * @param entity entity, at which remove component event is called
		 */
		template<typename TComponent> static void onRemove(const Entity& entity);

	private:

	    static FSceneManagerEditor* s_pSceneManagerEditor;

	};


}


#include "EventsComponentEntity.inl"


#endif // !MAR_ENGINE_F_COMPONENT_WIDGET_EVENTS_H
