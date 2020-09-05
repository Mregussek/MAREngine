/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

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
		private:
			static imgui_addons::ImGuiFileBrowser s_fileDialog;

		public:
			static void Filesystem_NewScene(const char* name);
			static void Filesystem_SaveScene(const char* name, ecs::Scene* scene_to_save);
			static void Filesystem_LoadScene(const char* name);

			static void Filesystem_LoadOBJfile(const char* name, ecs::Scene* scene);

		};


} }


#endif // !MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H
