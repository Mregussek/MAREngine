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

			static void Filesystem_SaveScene(bool& should_be_opened, ecs::Scene* scene_to_save);
			static void Filesystem_LoadScene(bool& should_be_opened);

		};


} }


#endif // !MAR_ENGINE_EDITOR_GUI_FILESYSTEM_H
