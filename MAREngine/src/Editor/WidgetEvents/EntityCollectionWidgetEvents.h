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


#ifndef MAR_ENGINE_F_ENTITY_COLLECTION_WIDGET_EVENTS_H
#define MAR_ENGINE_F_ENTITY_COLLECTION_WIDGET_EVENTS_H


namespace marengine {

	class Entity;
	class EntityCollection;


	class FEntityCollectionWidgetEvents {
	public:

		static void onCreateCollection();
		static void onDestroyCollection(const EntityCollection& collection);

		static void onEntityAddedToCollection(const EntityCollection& collection);
		static void onEntityRemovedFromCollection(const EntityCollection& collection, const Entity& entity);

		static void onSelectedCollection(const EntityCollection& collection);
		static void onUnselectedCollection();

		static void onSelectedEntityFromCollection(const Entity& entity);
		static void onUnselectedEntityFromCollection();

		static void onCopyCollection(const EntityCollection& collection);

		static void onSetVisibleCollection(const EntityCollection& collection);
		static void onSetInvisibleCollection(const EntityCollection& collection);

		template<typename T> static void onAddComponent(const EntityCollection& collection);
		template<typename T> static void onRemoveComponent(const EntityCollection& collection);
		template<typename T> static void onUpdateComponent(const EntityCollection& collection);

	};


}


#endif // !MAR_ENGINE_F_ENTITY_COLLECTION_WIDGET_EVENTS_H
