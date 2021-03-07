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


#include "EditorManager.h"
#include "EditorLogging.h"


namespace marengine {

	
	void FEditorManager::pushPanel(IEditorWidget* panel) {
		EDITOR_TRACE("F_EDITOR_MANAGER: pushing panel {}", panel->getTag());

		m_widgetPanels.emplace(m_widgetPanels.begin() + m_insertValue, panel);
		m_insertValue++;
	}

	void FEditorManager::popPanel(IEditorWidget* panel) {
		EDITOR_TRACE("F_EDITOR_MANAGER: popping panel {}", panel->getTag());

		auto it = std::find(m_widgetPanels.begin(), m_widgetPanels.begin() + m_insertValue, panel);
		if (it != (m_widgetPanels.begin() + m_insertValue)) {
			EDITOR_INFO("F_EDITOR_MANAGER: found panel {}, deleting...", panel->getTag());
			m_widgetPanels.erase(it);
			m_insertValue--;
		}
		else {
			EDITOR_WARN("F_EDITOR_MANAGER: could not find panel {}, so it is not going to be deleted", panel->getTag());
		}
	}

    void FEditorManager::onCreate() const {
        EDITOR_TRACE("F_EDITOR_MANAGER: destroying all panels...");
        std::for_each(m_widgetPanels.begin(), m_widgetPanels.end(), [](IEditorWidget* panel) {
            panel->onCreation();
        });
    }

	void FEditorManager::update() const {
		EDITOR_TRACE("F_EDITOR_MANAGER: updating all panels...");

		EDITOR_TRACE("F_EDITOR_MANAGER: calling beginFrame for all panels...");
		std::for_each(m_widgetPanels.begin(), m_widgetPanels.end(), [](IEditorWidget* panel) {
			panel->beginFrame();
			panel->onBeginFrame();
		});

		EDITOR_TRACE("F_EDITOR_MANAGER: calling updateFrame for all panels...");
		std::for_each(m_widgetPanels.begin(), m_widgetPanels.end(), [](IEditorWidget* panel) {
			panel->updateFrame();
			panel->onUpdateFrame();
		});

		EDITOR_TRACE("F_EDITOR_MANAGER: calling endFrame for all panels...");
		std::for_each(m_widgetPanels.begin(), m_widgetPanels.end(), [](IEditorWidget* panel) {
			panel->endFrame();
			panel->onEndFrame();
		});
	}

	void FEditorManager::destroy() const {
		EDITOR_TRACE("F_EDITOR_MANAGER: destroying all panels...");
		std::for_each(m_widgetPanels.begin(), m_widgetPanels.end(), [](IEditorWidget* panel) {
			panel->destroy();
			panel->onDestruction();
		});
	}


}
