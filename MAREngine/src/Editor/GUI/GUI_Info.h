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
