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


#ifndef MAR_ENGINE_F_EVENTS_CAMERA_ENTITY_H
#define MAR_ENGINE_F_EVENTS_CAMERA_ENTITY_H


namespace marengine {

	class RenderCamera;
	class Entity;


	/*
		FEventsCameraEntity is events class for updating camera, with which we can watch
		scene during gameplay. Should contain only static methods!
	*/
	class FEventsCameraEntity {
	public:

		/*
			Updates main camera during editor / game mode. You have to pass only RenderCamera.
			Then method will update needed buffers and use passed camera.
		*/
		static void onMainCameraUpdate(const RenderCamera* renderCamera);

		/*
			Updates main camera with given entity, only if it can be updated (FSceneManager must allow it).
			WARNING: Method assumes that entity contains main camera, it does not check if it is main one!
		*/
		static void onMainCameraUpdate(const Entity& entity);

		// Methed sets editor camera and pushes it to render pipeline.
		static void onEditorCameraSet(const RenderCamera* renderCamera);

		/*
		Method looks for main game camera (looks for entity with cameraComponent, which is main one) 
		and then pushes it to render pipeline.
		*/
		static void onGameCameraSet();

	};


}


#endif // !MAR_ENGINE_F_EVENTS_CAMERA_ENTITY_H
