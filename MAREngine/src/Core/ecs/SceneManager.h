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


#ifndef MAR_ENGINE_ECS_SCENE_MANAGER_H
#define MAR_ENGINE_ECS_SCENE_MANAGER_H


#include "../../mar.h"
#include "Components/Components.h"
#include "ScenePlayStorage.h"


namespace mar {
	namespace graphics { class RenderPipeline; }

	namespace ecs {

		class SceneEvents;
		class Scene;


		class SceneManager {
		// -------------------------------------------------------------
		// SCENE MANAGER PUBLIC METHODS
		// -------------------------------------------------------------

		public:
			SceneManager();
	
			void initialize();
			void shutdown();
			void update();

			void setScene(Scene* scene);
			Scene* getScene();

			bool isEditorMode() { return m_EditorMode; }
			bool isPlayMode() { return !m_EditorMode; }
			bool isPauseMode() { return m_PauseMode; }

			void setEditorMode() { m_EditorMode = true; }
			void setPlayMode() { m_EditorMode = false; initPlayMode(); }
			void setExitPlayMode() { m_EditorMode = true; unsetPauseMode(); exitPlayMode(); }
			void setPauseMode() { m_PauseMode = true; }
			void unsetPauseMode() { m_PauseMode = false; }

		// -------------------------------------------------------------
		// SCENE MANAGER PRIVATE METHODS
		// -------------------------------------------------------------

		private:
			void init(const std::vector<Entity>& entities, graphics::RenderPipeline& render_pipeline);

			void initPlayMode();
			void exitPlayMode();

			void updateEditorMode();
			void updatePlayMode();
			void updatePauseMode();

			void submitCamera(TransformComponent& tran, CameraComponent& cam);
			void calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam);

		// -------------------------------------------------------------
		// SCENE MANAGER MEMBERS
		// -------------------------------------------------------------

		private:
			friend class SceneEvents;
			Scene* m_scene;
			ScenePlayStorage m_playStorage;

		public:
			bool useEditorCamera;

		private:
			bool m_EditorMode;
			bool m_PauseMode;
		}; 


} }


#endif //!MAR_ENGINE_ECS_SCENE_MANAGER_H