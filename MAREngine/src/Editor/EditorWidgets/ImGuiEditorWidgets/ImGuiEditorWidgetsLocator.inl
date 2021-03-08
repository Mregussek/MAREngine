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


#ifndef MARENGINE_IMGUIEDITORWIDGETSLOCATOR_INL
#define MARENGINE_IMGUIEDITORWIDGETSLOCATOR_INL


#include "ImGuiEditorWidgetsLocator.h"

// Implementations
#include "Impl/DebugImGuiWidget.h"
#include "Impl/EnvironmentPropertiesImGuiWidget.h"
#include "Impl/InspectorImGuiWidget.h"
#include "Impl/MainImGuiWidget.h"
#include "Impl/MainMenuBarImGuiWidget.h"
#include "Impl/SceneHierarchyImGuiWidget.h"
#include "Impl/ScriptImGuiWidget.h"
#include "Impl/ViewportImGuiWidget.h"


namespace marengine {


	template<typename TImGuiEditorWidget>
	TImGuiEditorWidget* FImGuiEditorWidgetsLocator::retrieve() {
		return &m_imguiWidgetsRegistry.get<TImGuiEditorWidget>(m_imguiWidgetsContainer);
	}

	template<typename TImGuiEditorWidget>
	TImGuiEditorWidget* FImGuiEditorWidgetsLocator::emplace() {
		return &m_imguiWidgetsRegistry.emplace<TImGuiEditorWidget>(m_imguiWidgetsContainer);
	}


}


#endif //MARENGINE_IMGUIEDITORWIDGETSLOCATOR_INL