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


#ifndef MARENGINE_ENTITYIMGUIEDITOREVENTS_H
#define MARENGINE_ENTITYIMGUIEDITOREVENTS_H


#include "../../Core/ecs/Entity/IEntityEvents.h"


namespace marengine {

    class FSceneManagerEditor;
    class FServiceLocatorEditor;
    class FInspectorEditorWidget;
	class Entity;


	class FEntityEventsEditor : public IEntityEvents<FEntityEventsEditor> {
	public:

	    static void create(FServiceLocatorEditor* pServiceLocator);

        static void onCreateEntity();
        static void onDestroyEntity(const Entity& entity);

        static void onSelectedEntity(const Entity& entity);
        static void onUnselectedEntity(const Entity& entity);

        static void onCopyEntity(const Entity& entity);

        static void onSetVisibleEntity(const Entity& entity);
        static void onSetInvisibleEntity(const Entity& entity);

        static void onAssignChild(const Entity& entity, const Entity& child);
        static void onRemoveChild(const Entity& entity, const Entity& child);

        static void onCreateChild(const Entity& entity);
        static void onDestroyChild(const Entity& entity, const Entity& child);

	private:

        static FSceneManagerEditor* s_pSceneManagerEditor;
        static FInspectorEditorWidget* s_pInspectorWidget;

	};


}


#endif // !MARENGINE_ENTITYIMGUIEDITOREVENTS_H
