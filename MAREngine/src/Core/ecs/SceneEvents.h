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


namespace mar {
	namespace ecs {

		class Entity; class SceneManager; class EntityCollection;


		class SceneEvents {
			static SceneEvents instance;

		public:

			SceneEvents() = default;

			SceneManager* scene_manager{ nullptr };

			inline static SceneEvents& Instance() { return instance; }

			void updateTransform(Entity* e, int32_t i);

			void updateRenderables(Entity* e, int32_t i);

			void updatedCamera(Entity* e, int32_t i);

			void updatedColor(Entity* e, int32_t i);

			void updatedTexture2D(Entity* e, int32_t i);

			void updatedCubemap(Entity* e, int32_t i);

			void updatedLight(Entity* e, int32_t i);

			void updatedScript(Entity* e, int32_t i);

			void onEntityCopy();

			void onEntityRemove();

			void onCollectionTransformUpdate();

			void onCollectionRemove();

			void onCollectionOBJloaded(EntityCollection& collection);


		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
