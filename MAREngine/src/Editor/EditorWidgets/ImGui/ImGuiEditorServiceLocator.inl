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


#ifndef MARENGINE_IMGUIEDITORSERVICELOCATOR_INL
#define MARENGINE_IMGUIEDITORSERVICELOCATOR_INL


#include "ImGuiEditorServiceLocator.h"

// Implementations
#include "Impl/FilesystemPopUpWidgetImGui.h"
#include "Impl/DebugWidgetImGui.h"
#include "Impl/EnvironmentPropertiesWidgetImGui.h"
#include "Impl/InspectorWidgetImGui.h"
#include "Impl/MainWidgetImGui.h"
#include "Impl/MainMenuBarImGuiWidget.h"
#include "Impl/SceneHierarchyWidgetImGui.h"
#include "Impl/ScriptWidgetImGui.h"
#include "Impl/ViewportWidgetImGui.h"


namespace marengine {


	template<typename TImGuiService>
    TImGuiService* FImGuiEditorServiceLocator::retrieve() {
		return &m_imguiRegistry.get<TImGuiService>(m_imguiEntity);
	}

	template<typename TImGuiService>
    TImGuiService* FImGuiEditorServiceLocator::emplace() {
		return &m_imguiRegistry.emplace<TImGuiService>(m_imguiEntity);
	}


}


#endif //MARENGINE_IMGUIEDITORSERVICELOCATOR_INL