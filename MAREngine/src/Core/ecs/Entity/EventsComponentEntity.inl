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


#ifndef MAR_ENGINE_F_EVENTS_COMPONENT_ENTITY_INL
#define MAR_ENGINE_F_EVENTS_COMPONENT_ENTITY_INL


#include "EventsComponentEntity.h"
#include "Entity.h"
#include "Components.h"


namespace marengine {
	

	// Default implementations
	
	template<typename TComponent> static void FEventsComponentEntity::onAdd(const Entity& entity) {
		entity.template addComponent<TComponent>();
	}

	template<typename TComponent> static void FEventsComponentEntity::onUpdate(const Entity& entity) {

	}

	template<typename TComponent> static void FEventsComponentEntity::onRemove(const Entity& entity) {
		entity.template removeComponent<TComponent>();
	}


	// Here add declarations for specific component event implementations ...

	// CTransform
	template<> void FEventsComponentEntity::onUpdate<CTransform>(const Entity& entity);

	// CRenderable
	template<> void FEventsComponentEntity::onAdd<CRenderable>(const Entity& entity);
	template<> void FEventsComponentEntity::onUpdate<CRenderable>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<CRenderable>(const Entity& entity);

	// Point Light Component
	template<> void FEventsComponentEntity::onAdd<CPointLight>(const Entity& entity);
	template<> void FEventsComponentEntity::onUpdate<CPointLight>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<CPointLight>(const Entity& entity);

	// CCamera
	template<> void FEventsComponentEntity::onRemove<CCamera>(const Entity& entity);


}


#endif // !MAR_ENGINE_F_EVENTS_COMPONENT_ENTITY_INL
