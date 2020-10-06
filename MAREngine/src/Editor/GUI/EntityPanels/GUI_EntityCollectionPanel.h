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


#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H


#include "../../../mar.h"


namespace mar {
	namespace ecs { class EntityCollection; struct TagComponent; /* forward declarations */ }

	namespace editor {


		struct GUI_EntityCollectionPanel {
			static ecs::EntityCollection* currentCollection;
			static int32_t currentIndex;

			static void Scene_EntityCollection_Modify();

			static void reset();

		private:

			static void Scene_EntityCollection_PopUp(const char* collection_tag);
			static void Handle_TagComponent(ecs::TagComponent& tag);
			static void Handle_TransformComponent();

		};


} }



#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H