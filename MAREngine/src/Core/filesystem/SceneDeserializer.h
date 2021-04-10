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


#ifndef MAR_ENGINE_F_SCENE_DESERIALIZER_H
#define MAR_ENGINE_F_SCENE_DESERIALIZER_H


#include "../../mar.h"


namespace marengine {

	class Scene;
	class Entity;
    class FMeshManager;


	/**
	 * @class FSceneDeserializer SceneDeserializer.h "Core/filesystem/SceneDeserializer.h"
	 * @brief Class responsible for deserializing and loading whole scene from path.
	 */
	class FSceneDeserializer {
	public:

		/**
		 * @brief Method loads scene from given path. Make sure that it is correct path with
		 * .marscene.json extension.
		 * @warning If path is not correct, returns empty scene.
		 * @param path path, at which .marscene.json file should exist and it should be correct one.
		 * @return Returns loaded scene, if path was correct. Empty scene otherwise.
		 */
		static Scene* loadSceneFromFile(const std::string& path);

		static void passMeshFactory(FMeshManager* pManager) { s_pMeshManager = pManager; }

	private:

		static void loadEntity(const Entity& entity, uint32_t index, nlohmann::json& json,
                               const std::string& sceneName);


		static FMeshManager* s_pMeshManager;

	};


}


#endif // !MAR_ENGINE_F_SCENE_DESERIALIZER_H
