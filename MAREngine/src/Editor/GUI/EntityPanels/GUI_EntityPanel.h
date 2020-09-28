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


#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H


#include "../../../mar.h"


namespace mar {
	namespace ecs { class Entity; struct RenderableComponent; /* forward declarations */ }
	namespace graphics { struct RenderCamera; /* forward declarations */ }

	namespace editor {


		struct GUI_EntityPanel {
			static graphics::RenderCamera* render_cam;
			static ecs::Entity* currentEntity;
			static int32_t currentIndex;

			static void Scene_Entity_Modify(bool is_play_mode);
			static void reset();

		private:
			static void Scene_Entity_Modify_PopUp();

			static void Scene_Handle_TagComponent(bool window_focused);
			static void Scene_Handle_RenderableComponent(bool window_focused);
			static void Scene_Handle_TransformComponent(bool window_focused, bool is_play_mode);
			static void Scene_Handle_ScriptComponent(bool window_focused);
			static void Scene_Handle_CameraComponent(bool window_focused, bool is_play_mode);
			static void Scene_Handle_ColorComponent(bool window_focused, bool is_play_mode);
			static void Scene_Handle_Texture2DComponent(bool window_focused);
			static void Scene_Handle_TextureCubemapComponent(bool window_focused);
			static void Scene_Handle_LightComponent(bool window_focused, bool is_play_mode);

			// --------------------------------------------
			// TEMPLATES TO WRITE MORE REUSABLE CODE
			// --------------------------------------------

			template<typename T>
			static bool Button_ChooseRenderable(ecs::RenderableComponent& renderable, const char* buttonName);
		};



} }


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
