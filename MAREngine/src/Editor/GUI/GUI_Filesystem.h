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


#ifndef MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H
#define MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H


#include "../../mar.h"
#include "../EditorLogging.h"
#include "../Filesystem/EditorFilesystem.h"
#include "../../Core/ecs/Scene.h"
#include "../../Engine.h"


namespace mar {
	namespace editor {


		struct GUI_Filesystem {

			static void SetOpenNewScene(const char* name);
			static void Filesystem_NewScene(const char* name);

			static void SetOpenSaveScene(const char* name);
			static void Filesystem_SaveScene(const char* name, ecs::Scene* scene_to_save);

			static void SetOpenLoadScene(const char* name);
			static void Filesystem_LoadScene(const char* name);

			static void SetOpenLoadOBJfile(const char* name);
			static void Filesystem_LoadOBJfile(const char* name, ecs::Scene* scene);

			static void Filesystem_AssetManager(const char* name);

		};


} }


#endif // !MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H
