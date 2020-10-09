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


#ifndef MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H
#define MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H


#include "../../mar.h"

namespace mar::ecs { 
	class Scene; class EntityCollection; class Entity;	/* forward declarations */ 
}
namespace mar::editor {


	class Filesystem_Saving {
	public:
		static void saveScene(std::ofstream& ss, ecs::Scene* scene);
		static void saveCollection(std::ofstream& ss, const ecs::EntityCollection& collection);
		static void saveEntity(std::ofstream& ss, const ecs::Entity& entity);
	};


}


#endif // !MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H
