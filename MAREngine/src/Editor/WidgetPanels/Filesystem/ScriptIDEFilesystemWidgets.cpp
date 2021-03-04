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


#include "ScriptIDEFilesystemWidgets.h"
#include "../../EditorLogging.h"
#include "FilesystemWidgets.h"
#include "../ScriptEditor/ScriptIDEWidget.h"
#include "../../../Core/filesystem/FileManager.h"


namespace marengine {


	WScriptIDEFilesystemWidgets* WScriptIDEFilesystemWidgets::Instance{ nullptr };


	void WScriptIDEFilesystemWidgets::create() {
		Instance = this;
	}

	void WScriptIDEFilesystemWidgets::updateFrame() {
		constexpr auto createNewPythonScriptOnEditorCallback = [](const std::string& path, const std::string& filename) {
			const std::string& defaultSourceCode{ WScriptIDE::Instance->getDefaultEditorSourceCode() };
			FFileManager::saveAsFile(defaultSourceCode, path.c_str());

			WScriptIDE::Instance->setEditorTitle(filename);
			WScriptIDE::Instance->setEditorCode(defaultSourceCode);
			WScriptIDE::Instance->setPathToScript(path);
		};

		constexpr auto saveAsPythonScriptOnEditorCallback = [](const std::string& path, const std::string& filename) {
			const std::string& sourceCode{ WScriptIDE::Instance->getEditorSourceCode() };
			FFileManager::saveAsFile(sourceCode, path.c_str());

			WScriptIDE::Instance->setEditorTitle(filename);
			WScriptIDE::Instance->setEditorCode(sourceCode);
			WScriptIDE::Instance->setPathToScript(path);
		};

		constexpr auto openInEditorScriptCallback = [](const std::string& path, const std::string& filename) {
			std::string sourceCode{};
			FFileManager::loadFile(sourceCode, path.c_str());

			WScriptIDE::Instance->setEditorTitle(filename);
			WScriptIDE::Instance->setEditorCode(sourceCode);
			WScriptIDE::Instance->setPathToScript(path);
		};

		WFilesystemWidgets::Instance->displaySaveWidget(createNewEditorScript, py, createNewPythonScriptOnEditorCallback);
		WFilesystemWidgets::Instance->displaySaveWidget(saveAsEditorScript, py, saveAsPythonScriptOnEditorCallback);
		WFilesystemWidgets::Instance->displayOpenWidget(openInEditorScript, py, openInEditorScriptCallback);
	}

	void WScriptIDEFilesystemWidgets::openCreateNewEditorScriptWidget() const {
		WFilesystemWidgets::Instance->openWidget(createNewEditorScript.c_str());
	}

	void WScriptIDEFilesystemWidgets::openSaveAsEditorScriptWidget() const {
		WFilesystemWidgets::Instance->openWidget(saveAsEditorScript.c_str());
	}

	void WScriptIDEFilesystemWidgets::openOpenInEditorScriptWidget() const {
		WFilesystemWidgets::Instance->openWidget(openInEditorScript.c_str());
	}


}
