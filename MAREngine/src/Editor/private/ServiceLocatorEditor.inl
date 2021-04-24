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


#ifndef MARENGINE_SERVICELOCATOREDITOR_INL
#define MARENGINE_SERVICELOCATOREDITOR_INL


#include "../public/ServiceLocatorEditor.h"

// ImGui Implementations
#include "ImGui/FilesystemPopUpImGui.h"
#include "ImGui/DebugImGui.h"
#include "ImGui/EnvironmentPropertiesImGui.h"
#include "ImGui/InspectorImGui.h"
#include "ImGui/MainImGui.h"
#include "ImGui/MainMenuBarImGui.h"
#include "ImGui/SceneHierarchyImGui.h"
#include "ImGui/ScriptImGui.h"
#include "ImGui/ViewportImGui.h"
#include "ImGui/ContentBrowserImGui.h"


namespace marengine {


	template<typename TService>
    TService* FServiceLocatorEditor::retrieve() {
		return &m_registry.get<TService>(m_entity);
	}

	template<typename TService>
    TService* FServiceLocatorEditor::emplace() {
		return &m_registry.emplace<TService>(m_entity);
	}

    template<EEditorContextType TType>
    void FServiceLocatorEditor::create() { }


    template<> void FServiceLocatorEditor::create<EEditorContextType::IMGUI>();


}


#endif //MARENGINE_SERVICELOCATOREDITOR_INL