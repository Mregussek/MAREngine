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


namespace marengine {

	class RenderCamera;
	class Entity; 
	struct RenderableComponent;


	class GUI_EntityPanel {
	public:

		static GUI_EntityPanel* Instance() { return s_instance; }

		void initialize();
		void reset();

		void update(bool isPlayMode) const;
		
		void setCurrentEntity(const Entity& entity);
		const Entity& getCurrentEntity() const;

	private:

		void displayPlayMode() const;
		void displayEditorMode() const;

		void popUpMenu() const;

		void handleTransformComponent() const;
		void handleRenderableComponent() const;
		void handleCameraComponent() const;
		void handleColorComponent() const;
		void handleTexture2DComponent() const;
		void handleTextureCubemapComponent() const;
		void handleLightComponent() const;

		template<typename T>
		bool Button_ChooseRenderable(RenderableComponent& renderable, const char* buttonName) const;


		static GUI_EntityPanel* s_instance;

		const Entity* currentEntity;
		bool m_initialized{ false };

	};



}


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
