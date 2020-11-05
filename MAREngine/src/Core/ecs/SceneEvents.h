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


namespace mar::graphics { class RenderCamera; }
namespace mar::ecs {

	class Entity; 
	class EntityCollection;
	class SceneManager;
	struct TransformComponent;


	class SceneEvents {
	public:

		SceneEvents() = default;

		static SceneEvents& Instance();
		void setSceneManager(SceneManager& manager);

		void onTransformUpdate(const Entity* e) const;

		void onRenderableAdd() const;
		void onRenderableUpdate(const Entity* e) const;
		void onRenderableRemove() const;

		void onCameraAdd() const;
		void onCameraUpdate(const Entity* e) const;
		void onCameraRemove() const;

		void onColorAdd() const;
		void onColorUpdate(const Entity* e) const;
		void onColorRemove() const;

		void onTexture2DAdd() const;
		void onTexture2DUpdate(const Entity* e) const;
		void onTexture2DRemove() const;

		void onTextureCubemapAdd() const;
		void onTextureCubemapUpdate(const Entity* e) const;
		void onTextureCubemapRemove() const;

		void onLightAdd() const;
		void onLightUpdate(const Entity* e) const;
		void onLightRemove() const; 

		void onScriptAdd() const;
		void onScriptUpdate(const Entity* e) const;
		void onScriptRemove() const;

		void onEditorCameraSet(graphics::RenderCamera* camera) const;
		void onGameCameraSet() const;

		void onEntityCopy() const;
		void onEntityRemove() const;

		void onCollectionTransformUpdate(const EntityCollection* collection, const TransformComponent& transform) const;
		void onCollectionTransformReset(const EntityCollection* collection) const;
		void onCollectionCopy() const;
		void onCollectionRemove() const;
		void onCollectionOBJloaded(const EntityCollection& collection) const;

	private:

		static SceneEvents s_instance;
		SceneManager* m_sceneManager{ nullptr };

	};


}


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
