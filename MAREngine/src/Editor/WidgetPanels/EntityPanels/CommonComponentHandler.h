/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_PANEL_COMMON_COMPONENT_HANDLER_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_PANEL_COMMON_COMPONENT_HANDLER_H


#include "../../../mar.h"


namespace marengine {

	struct TagComponent; 
	struct PythonScriptComponent;
	class Entity; 


	struct CommonComponentHandler {

		static void handleTagComponent(TagComponent& tag);
		static void handleScriptComponent(const Entity& entity);

		static bool drawVec3Control(const char* label, maths::vec3& values, float resetValue, float columnWidth, float minValue, float maxValue);
		static bool drawVec4Control(const char* label, maths::vec4& values, float resetValue, float columnWidth);

	private:

		static void handleScriptComponent(PythonScriptComponent& script);

	};


}


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_PANEL_COMMON_COMPONENT_HANDLER_H
