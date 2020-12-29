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


#include "InfoWidget.h"
#include "../../../mar.h"
#include "../../EditorLogging.h"


namespace marengine {


	WInfoWidget* WInfoWidget::Instance{ nullptr };

	void WInfoWidget::create() {
		Instance = this;
	}

	void WInfoWidget::updateFrame() {
		if (shouldInfoBeDisplayed) {
			shouldInfoBeDisplayed = actualDisplayInfoWindow();
		}

		if (shouldInstructionBeDisplayed) {
			shouldInstructionBeDisplayed = actualDisplayInstructionWindow();
		}
	}

	void WInfoWidget::displayInfo() {
		shouldInfoBeDisplayed = true;
	}

	void WInfoWidget::displayInstruction() {
		shouldInstructionBeDisplayed = true;
	}

	bool WInfoWidget::actualDisplayInfoWindow() const {
		ImGui::Begin("About");

		const char* aboutEngine = 
			"MAREngine is an educational 3D game engine written in modern C++.\n"
			"It is created with a dream of easy and intuitive interface for everyone.\n"
			"You can download MAREngine completely freely and release your creativity - create games you imagine.";

		const char* aboutAuthor =
			"Mateusz Rzeczyca is C++ / Python programmer and ethusiast of Augmented Reality and Machine Learning.\n"
			"\n"
			"I am delighted that you are using MAREngine! Thank you!";

		ImGui::Text("About Engine");
		ImGui::Text(aboutEngine);

		ImGui::Separator();

		ImGui::Text("About Author");
		ImGui::Text(aboutAuthor);

		if (ImGui::Button("Close")) {
			ImGui::End();
			return false;
		}

		ImGui::End();
		return true;

		EDITOR_TRACE("GUI: menu_info");
	}

	bool WInfoWidget::actualDisplayInstructionWindow() const {
		ImGui::Begin("Instructions");

		if (ImGui::Button("Close")) {
			ImGui::End();
			return false;
		}

		ImGui::End();
		return true;

		EDITOR_TRACE("GUI: menu_instruction");
	}


}
