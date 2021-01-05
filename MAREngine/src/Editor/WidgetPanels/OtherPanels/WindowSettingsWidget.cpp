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


#include "WindowSettingsWidget.h"
#include "../../../mar.h"
#include "../../../Window/Window.h"


namespace marengine {

	WWindowSettingsWidget* WWindowSettingsWidget::Instance{ nullptr };


	void WWindowSettingsWidget::create() {
		Instance = this;
	}

	void WWindowSettingsWidget::updateFrame() {
		if (m_displaySettingsWindow) {
			m_displaySettingsWindow = actualDisplaySettingsWindow();
		}
	}

	void WWindowSettingsWidget::openSettingsWindowWidget() {
		m_displaySettingsWindow = true;
	}

	bool WWindowSettingsWidget::actualDisplaySettingsWindow() const {
		ImGui::Begin("Window Settings");

		static bool verticalSync{ true };
		ImGui::Checkbox("Window Vertical Synchronization (when turned off increases FPS)", &verticalSync);

		if (verticalSync) {
			Window::setVerticalSync(1);
		}
		else {
			Window::setVerticalSync(0);
		}

		if (ImGui::Button("Close")) {
			ImGui::End();
			return false;
		}

		ImGui::End();
		return true;
	}


}
