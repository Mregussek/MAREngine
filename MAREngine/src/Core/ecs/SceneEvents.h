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
	public:

		SceneEvents() = default;

		static SceneEvents& Instance() { return s_instance; }
		void setSceneManager(SceneManager& manager) { m_sceneManager = &manager; }

		void onTransformUpdate(const Entity* e);

		void onRenderableAdd();
		void onRenderableUpdate(const Entity* e);
		void onRenderableRemove();

		void onCameraAdd();
		void onCameraUpdate(const Entity* e);
		void onCameraRemove();

		void onColorAdd();
		void onColorUpdate(const Entity* e);
		void onColorRemove();

		void onTexture2DAdd();
		void onTexture2DUpdate(const Entity* e);
		void onTexture2DRemove();

		void onTextureCubemapAdd();
		void onTextureCubemapUpdate(const Entity* e);
		void onTextureCubemapRemove();

		void onLightAdd();
		void onLightUpdate(const Entity* e);
		void onLightRemove();

		void onScriptAdd();
		void onScriptUpdate(const Entity* e);
		void onScriptRemove();

		void onEntityCopy();
		void onEntityRemove();
		void onCollectionTransformUpdate();
		void onCollectionRemove();
		void onCollectionOBJloaded(const EntityCollection& collection);

	private:

		static SceneEvents s_instance;
		SceneManager* m_sceneManager{ nullptr };

	};


}


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
