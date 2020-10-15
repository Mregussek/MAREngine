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


#ifndef MAR_ENGINE_ECS_SCENE_EVENTS_H
#define MAR_ENGINE_ECS_SCENE_EVENTS_H


#include "../../mar.h"


namespace mar::ecs {

	class Entity; 
	class EntityCollection;
	class SceneManager;


	class SceneEvents {
		static SceneEvents instance;

	public:

		SceneEvents() = default;

		SceneManager* scene_manager{ nullptr };

		inline static SceneEvents& Instance() { return instance; }

		void updateTransform(const Entity* e);
		void updateRenderables(const Entity* e);
		void updatedCamera(const Entity* e);
		void updatedColor(const Entity* e);
		void updatedTexture2D(const Entity* e);
		void updatedCubemap(const Entity* e);
		void updatedLight(const Entity* e);
		void updatedScript(const Entity* e);
		void onEntityCopy();
		void onEntityRemove();
		void onCollectionTransformUpdate();
		void onCollectionRemove();
		void onCollectionOBJloaded(const EntityCollection& collection);


	};


}


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
