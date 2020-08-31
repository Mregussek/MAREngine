/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_GUI_INFO_H
#define MAR_ENGINE_EDITOR_GUI_INFO_H


#include "../../mar.h"
#include "../EditorLogging.h"


namespace mar {
	namespace editor {


		struct GUI_Info {
			static void Menu_Info(bool& should_be_open);
			static void Menu_Instruction(bool& should_be_open);
		};


} }


#endif // !MAR_ENGINE_EDITOR_GUI_INFO_H
