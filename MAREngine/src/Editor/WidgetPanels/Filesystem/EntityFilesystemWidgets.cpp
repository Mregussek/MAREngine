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


#include "EntityFilesystemWidgets.h"
#include "FilesystemWidgets.h"
#include "../../EditorLogging.h"
#include "../EntityPanels/EntityWidgetPanel.h"
#include "../../../ProjectManager.h"
#include "../../../Core/ecs/Components/Components.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/graphics/Mesh/MeshCreator.h"
#include "../../WidgetEvents/EventsEntityWidget.h"
#include "../../../Core/graphics/RenderAPI/RenderPipeline.h"


namespace marengine {


	WEntityFilesystemWidgets* WEntityFilesystemWidgets::Instance{ nullptr };


	void WEntityFilesystemWidgets::create() {
		Instance = this;
	}

	void WEntityFilesystemWidgets::updateFrame() {
		constexpr auto loadOBJcallback = [](const std::string& path, const std::string& filename) {
			EDITOR_TRACE("WEntityFilesystemWidgets:49:EntityFilesystemWidgets.cpp: Loading object {} {} and assining to newly created entity", path, filename);
			
			const Entity& entity{ FSceneManagerEditor::Instance->getScene()->createEntity() };
			MeshCreator::loadOBJ(filename, path, entity);
			FEventsEntityWidget::Instance->onSelectedEntity(entity);
			RenderPipeline::Instance->pushEntityToPipeline(entity);
			RenderPipeline::Instance->onBatchesReadyToDraw();
		};

		constexpr auto assignScriptCallback = [](const std::string& path, const std::string& filename) {
			const Entity& entity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
			EDITOR_WARN("WEntityFilesystemWidgets:59:EntityFilesystemWidgets.cpp: Assigning script {} {} to entity {} ", path, filename, entity.getComponent<TagComponent>().tag);

			const std::string& assetsPath{ ProjectManager::Instance->getAssetsPath() };
			auto& pythonScriptComponent{ entity.getComponent<PythonScriptComponent>() };
		};

		constexpr auto createAndAssignScriptCallback = [](const std::string& path, const std::string& filename) {
			const Entity& entity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
			EDITOR_WARN("WEntityFilesystemWidgets:67:EntityFilesystemWidgets.cpp: creating and assigning {} {} to entity {} ", path, filename, entity.getComponent<TagComponent>().tag);

			const std::string& assetsPath{ ProjectManager::Instance->getAssetsPath() };
			auto& pythonScriptComponent{ entity.getComponent<PythonScriptComponent>() };
		};

		WFilesystemWidgets::Instance->displayOpenWidget(WidgetNames.loadObj, FileExtensions.obj, loadOBJcallback);
		WFilesystemWidgets::Instance->displayOpenWidget(WidgetNames.assignPythonScript, FileExtensions.py, assignScriptCallback);
		WFilesystemWidgets::Instance->displayOpenWidget(WidgetNames.createAndAssignPythonScript, FileExtensions.py, createAndAssignScriptCallback);
	}

	void WEntityFilesystemWidgets::openLoadOBJWidget() const {
		WFilesystemWidgets::Instance->openWidget(WidgetNames.loadObj.c_str());
	}

	void WEntityFilesystemWidgets::openCreateAndAssignPythonScriptWidget() const {
		WFilesystemWidgets::Instance->openWidget(WidgetNames.createAndAssignPythonScript.c_str());
	}

	void WEntityFilesystemWidgets::openAssignPythonScriptWidget() const {
		WFilesystemWidgets::Instance->openWidget(WidgetNames.assignPythonScript.c_str());
	}


}