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

		void initialize();
		void reset();

		void update(bool isPlayMode);
		
		void setRenderCam(graphics::RenderCamera* renderCam);
		void setCurrentEntity(const ecs::Entity& entity);
		const ecs::Entity& getCurrentEntity() const;

	private:

		void displayPlayMode();
		void displayEditorMode();

		void popUpMenu();

		void handleTagComponent();
		void handleTransformComponent();
		void handleScriptComponent();
		void handleRenderableComponent();
		void handleCameraComponent();
		void handleColorComponent();
		void handleTexture2DComponent();
		void handleTextureCubemapComponent();
		void handleLightComponent();

		template<typename T>
		bool Button_ChooseRenderable(ecs::RenderableComponent& renderable, const char* buttonName);

		static GUI_EntityPanel* s_instance;

		graphics::RenderCamera* render_cam{ nullptr };
		const ecs::Entity* currentEntity;
		bool m_initialized{ false };

	};



}


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
