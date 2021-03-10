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


#ifndef MAR_ENGINE_F_SCENE_MANAGER_EDITOR_H
#define MAR_ENGINE_F_SCENE_MANAGER_EDITOR_H


#include "../../mar.h"
#include "ScenePlayStorage.h"


namespace marengine {

	class Scene;
	class Entity;

	
	/**
	 * @class FSceneManagerEditor SceneManagerEditor.h "Core/ecs/SceneManagerEditor.h"
	 * @brief Scene Manager for Editor mode. With this manager
	 * you manage Editor / Game camera display, Editor / Play mode on Viewport.
	 * In SceneLayer static member Instance is set.
	 */
	class FSceneManagerEditor {
	public:

		static FSceneManagerEditor* Instance;

		/// @brief Default constructor
		FSceneManagerEditor() = default;

		/// @brief Initializes whole scene, pushes every entity for batching and afterwards calls draw ready state.
		void initialize() const;

		/**
		 * @brief Updates Scene in SceneManager's state. During EditorMode there is no need to update the scene,
		 * everything should operate on events. During PlayMode we need to call update PythonScripts and then 
		 * update buffers every time.
		 */
		void update();

		/// @brief Closes SceneManager
		void close();

		/**
		 * @brief Sets given scene, which will be then managed.
		 * @param scene scene, which will be managed
		 */
		void setScene(Scene* scene);
		
		/**
		 * @brief Method returns scene that is managed.
		 * @return Returns currently managed scene
		 */
		MAR_NO_DISCARD Scene* getScene();

		/// @brief Sets Editor Mode (for update state)
		void setEditorMode();

		/**
		 * @brief Returns value of m_EditorMode variable.
		 * @return Returns true, if scene is in editor mode 
		 */
        MAR_NO_DISCARD bool isEditorMode() const;

		/**
		 * @brief Returns value of !m_EditorMode variable.
		 * @return Returns true, if scene is in play mode 
		 */
        MAR_NO_DISCARD bool isPlayMode() const;

		/**
		 * @brief Returns value of m_PauseMode variable.
		 * @return Returns true, if scene is in pause mode 
		 */
        MAR_NO_DISCARD bool isPauseMode() const;

		/// @brief Sets Play mode
		void setPlayMode();
		
		/// @brief Set exit play mode state
		void setExitPlayMode();

		/// @brief Set Pause mode
		void setPauseMode();
		
		/// @brief Set exit pause mode state
		void setExitPauseMode();
		
		/// @brief Use Editor Camera to watch scene at viewport
		void useEditorCamera();

		/// @brief Use Game Camera to watch scene at viewport
		void useGameCamera();

		/**
		 * @brief 
		 * @return Returns true, if using editor camera to watch scene at viewport 
		 */
        MAR_NO_DISCARD bool usingEditorCamera() const;

		/**
		 * @brief Returns value of !m_EditorCamera variable.
		 * @return Returns true, if using game camera to watch scene at viewport 
		 */
        MAR_NO_DISCARD bool usingGameCamera() const;

	private:

		void initPlayMode();
		void updatePlayMode();
		void updatePauseMode();
		void updateEntityInPlaymode(const Entity& entity);
		void exitPlayMode();


		Scene* m_pScene{ nullptr };
		
		bool m_EditorCamera{ true };
		bool m_EditorMode{ true };
		bool m_PauseMode{ false };

	}; 


}


#endif //!MAR_ENGINE_F_SCENE_MANAGER_EDITOR_H