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


#ifndef MAR_ENGINE_F_ALL_WIDGET_PANELS_H
#define MAR_ENGINE_F_ALL_WIDGET_PANELS_H


#include "GeneralWidgetPanel.h"
#include "MenuBars/MainMenuBarWidget.h"
#include "Filesystem/FilesystemWidgets.h"
#include "ScriptIDEWidget.h"
#include "Viewport/ViewportWidget.h"
#include "ScenePropertiesPanel/ScenePropertiesWidget.h"
#include "EntityPanels/EntityWidgetPanel.h"
#include "EntityPanels/EntityCollectionWidgetPanel.h"
#include "EntityPanels/SceneHierarchyWidget.h"
#include "OtherPanels/ThemeWidget.h"
#include "OtherPanels/StatisticsWidget.h"


namespace marengine {


	struct FAllWidgetPanels {

		FAllWidgetPanels() = default;

		WGeneralWidgetPanel general{};
		WViewportWidget viewport{};
		WMainMenuBarWidget mainMenuBar{};
		WFilesystemWidgets filesystem{};
		WEntityWidgetPanel entity{};
		WEntityCollectionWidgetPanel entityCollection{};
		WSceneHierarchyWidget sceneHierarchy{};
		WScriptIDE textEditor{};
		WScenePropertiesWidget sceneProperties{};
		WThemeWidget theme{};
		WStatisticsWidget statistics{};

	};


}


#endif // !MAR_ENGINE_F_ALL_WIDGET_PANELS_H
