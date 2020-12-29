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


#include "CommonComponentHandler.h"
#include "../ScriptEditor/ScriptIDEWidget.h"
#include "../Filesystem/FilesystemWidgets.h"
#include "../../EditorLogging.h"
#include "../../../Core/ecs/Components/Components.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"


namespace marengine {


	void CommonComponentHandler::handleTagComponent(TagComponent& tag) {
		constexpr size_t inputSize = 50;
		static char collectionName[inputSize]{ "" };

		std::fill(std::begin(collectionName), std::end(collectionName), '\0');
		std::copy(tag.tag.begin(), tag.tag.end(), collectionName);

		if (ImGui::InputText(" - tag", collectionName, inputSize)) {
			tag.tag = std::string(collectionName);
		}
	}

	void CommonComponentHandler::handleScriptComponent(const Entity& entity) {
		if (ImGui::MenuItem("Remove Script")) {
			entity.removeComponent<ScriptComponent>();
			WScriptIDE::Instance->reset();
			return;
		}

		auto& script = entity.getComponent<ScriptComponent>();
		handleScriptComponent(script);
	}

	void CommonComponentHandler::handleScriptComponent(const EntityCollection& collection) {
		if (ImGui::MenuItem("Remove Script")) {
			collection.removeComponent<ScriptComponent>();
			WScriptIDE::Instance->reset();
			return;
		}

		auto& script = collection.getComponent<ScriptComponent>();
		handleScriptComponent(script);
	}

	void CommonComponentHandler::handleScriptComponent(ScriptComponent& script) {
		ImGui::Text("Current script: %s", script.script.c_str());

		if (ImGui::Button("Create new script")) { WScriptIDE::Instance->setCreatingNewScript(); }

		ImGui::SameLine();

		if (ImGui::Button("Load from file")) { WScriptIDE::Instance->setLoadingScript(); }

		ImGui::SameLine();

		if (ImGui::Button("Assign script to entity")) { WFilesystemWidgets::Instance->openAssigningScriptWindow(); }

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling script component");
	}


}
