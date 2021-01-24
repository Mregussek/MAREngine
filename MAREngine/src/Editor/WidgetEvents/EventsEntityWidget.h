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


#ifndef MAR_ENGINE_F_EVENTS_ENTITY_WIDGET_H
#define MAR_ENGINE_F_EVENTS_ENTITY_WIDGET_H


#include "../../Core/ecs/Entity/IEntityEvents.h"


namespace marengine {

	class Entity;


	class FEventsEntityWidget : public IEntityEvents {
	public:

		static FEventsEntityWidget* Instance;

		virtual void onCreateEntity() const override;
		virtual void onDestroyEntity(const Entity& entity) const override;

		virtual void onSelectedEntity(const Entity& entity) const override;
		virtual void onUnselectedEntity(const Entity& entity) const override;

		virtual void onCopyEntity(const Entity& entity) const override;

		virtual void onSetVisibleEntity(const Entity& entity) const override;
		virtual void onSetInvisibleEntity(const Entity& entity) const override;

		virtual void onAssignChild(const Entity& entity, const Entity& child) const override;
		virtual void onRemoveChild(const Entity& entity, const Entity& child) const override;

		virtual void onCreateChild(const Entity& entity) const override;
		virtual void onDestroyChild(const Entity& entity, const Entity& child) const override;

	};


}


#endif // !MAR_ENGINE_F_ENTITY_WIDGET_EVENTS_H
