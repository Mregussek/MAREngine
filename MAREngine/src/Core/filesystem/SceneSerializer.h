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


#ifndef MAR_ENGINE_F_SCENE_SERIALIZER_H
#define MAR_ENGINE_F_SCENE_SERIALIZER_H


#include "../../mar.h"


namespace marengine {

	class Scene;
	class Entity;


	/**
	 * @class FSceneSerializer SceneSerializer.h "Core/filesystem/SceneSerializer.h"
	 * @brief Class reponsible for serializing scene into small parts and saving theme into file.
	 */
	class FSceneSerializer {
	public:

		/**
		 * @brief Serializes given scene and then saves it into given path. Make sure that path
		 * is ending with .marscene.json extension.
		 * @warning if path is incorrect, it immedietaly returns and displays error.
		 * @param path path at which scene will be saved
		 * @param scene scene, that will be saved
		 */
		static void saveSceneToFile(const char* path, Scene* scene);

	};


}


#endif // !MAR_ENGINE_F_SCENE_SERIALIZER_H
