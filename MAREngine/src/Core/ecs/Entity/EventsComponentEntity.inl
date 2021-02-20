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
#include "../Components/Components.h"


namespace marengine {
	

	// Default implementations
	
	template<typename TComponent> static void FEventsComponentEntity::onAdd(const Entity& entity) {
		entity.template addComponent<T>();
	}

	template<typename TComponent> static void FEventsComponentEntity::onUpdate(const Entity& entity) {

	}

	template<typename TComponent> static void FEventsComponentEntity::onRemove(const Entity& entity) {
		entity.template removeComponent<T>();
	}


	// Here add declarations for specific component event implementations ...

	// TransformComponent
	template<> void FEventsComponentEntity::onUpdate<TransformComponent>(const Entity& entity);

	// RenderableComponent
	template<> void FEventsComponentEntity::onAdd<RenderableComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onUpdate<RenderableComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<RenderableComponent>(const Entity& entity);

	// ColorComponent
	template<> void FEventsComponentEntity::onAdd<ColorComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onUpdate<ColorComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<ColorComponent>(const Entity& entity);

	// Texture2D Component
	template<> void FEventsComponentEntity::onUpdate<Texture2DComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<Texture2DComponent>(const Entity& entity);

	// Texture CubemapComponent
	template<> void FEventsComponentEntity::onUpdate<TextureCubemapComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<TextureCubemapComponent>(const Entity& entity);

	// Point Light Component
	template<> void FEventsComponentEntity::onAdd<PointLightComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onUpdate<PointLightComponent>(const Entity& entity);
	template<> void FEventsComponentEntity::onRemove<PointLightComponent>(const Entity& entity);

	// CameraComponent
	template<> void FEventsComponentEntity::onRemove<CameraComponent>(const Entity& entity);


}


#endif // !MAR_ENGINE_F_EVENTS_COMPONENT_ENTITY_INL
