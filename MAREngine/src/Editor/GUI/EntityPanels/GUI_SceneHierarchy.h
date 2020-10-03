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


#ifndef MAR_ENGINE_GUI_SCENE_HIERARCHY_PANEL_H
#define MAR_ENGINE_GUI_SCENE_HIERARCHY_PANEL_H


#include "../../../mar.h"


namespace mar {
	namespace ecs { class SceneManager; }

	namespace editor {


		class GUI_SceneHierarchy {
		public:

			static void Scene_Hierarchy(ecs::SceneManager* manager);
			
		private:

			static void Scene_Hierarchy_PopUp(ecs::SceneManager* manager);
			static void Scene_Statistics(ecs::SceneManager* manager);
		};


} }


#endif // !MAR_ENGINE_GUI_SCENE_HIERARCHY_PANEL_H