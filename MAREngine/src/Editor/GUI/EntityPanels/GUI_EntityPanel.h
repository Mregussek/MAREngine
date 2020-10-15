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
	/* forward declarations */
	namespace ecs { class Entity; struct RenderableComponent; }
	namespace graphics { class RenderCamera; }
}
namespace mar::editor {


	class GUI_EntityPanel {
	public:

		static GUI_EntityPanel* Instance() { return s_instance; }

		void initialize() {
			if (m_initialized) { return; }

			s_instance = this;
			m_initialized = true;
		}

		void Scene_Entity_Modify(bool is_play_mode);
		void reset();

		void setRenderCam(graphics::RenderCamera* renderCam) { render_cam = renderCam; }
		void setCurrentEntity(const ecs::Entity& entity) { currentEntity = &entity; }
		const ecs::Entity& getCurrentEntity() const { return *currentEntity; }

	private:
		void Scene_Entity_Modify_PopUp();

		void Scene_Handle_TagComponent(bool window_focused);
		void Scene_Handle_RenderableComponent(bool window_focused);
		void Scene_Handle_TransformComponent(bool window_focused, bool is_play_mode);
		void Scene_Handle_ScriptComponent(bool window_focused);
		void Scene_Handle_CameraComponent(bool window_focused, bool is_play_mode);
		void Scene_Handle_ColorComponent(bool window_focused, bool is_play_mode);
		void Scene_Handle_Texture2DComponent(bool window_focused);
		void Scene_Handle_TextureCubemapComponent(bool window_focused);
		void Scene_Handle_LightComponent(bool window_focused, bool is_play_mode);

		template<typename T>
		bool Button_ChooseRenderable(ecs::RenderableComponent& renderable, const char* buttonName);

		static GUI_EntityPanel* s_instance;

		graphics::RenderCamera* render_cam{ nullptr };
		const ecs::Entity* currentEntity;
		bool m_initialized{ false };

	};



}


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
