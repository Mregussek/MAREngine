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


#ifndef MAR_ENGINE_F_SCENE_MANAGER_EDITOR_H
#define MAR_ENGINE_F_SCENE_MANAGER_EDITOR_H


#include "../../mar.h"
#include "ScenePlayStorage.h"


namespace marengine {

	class Scene;
	class Entity;


	class FSceneManagerEditor {
	public:

		static FSceneManagerEditor* Instance;

		FSceneManagerEditor() = default;

		void initialize() const;
		void update();
		void close();

		void setScene(Scene* scene);
		Scene* getScene();

		void setEditorMode();
		bool isEditorMode() const;

		void setPlayMode();
		bool isPlayMode() const;
		void setExitPlayMode();

		void setPauseMode();
		bool isPauseMode() const;
		void setExitPauseMode();
		
		void useEditorCamera();
		void useGameCamera();
		bool usingEditorCamera() const;

	private:

		void initPlayMode();
		void updatePlayMode();
		void updatePauseMode();
		void updateEntityInPlaymode(const Entity& entity);
		void exitPlayMode();


		ScenePlayStorage m_playStorage;
		Scene* m_scene{ nullptr };
		
		bool m_EditorCamera{ true };
		bool m_EditorMode{ true };
		bool m_PauseMode{ false };

	}; 


}


#endif //!MAR_ENGINE_F_SCENE_MANAGER_EDITOR_H