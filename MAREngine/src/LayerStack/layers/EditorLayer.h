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


#ifndef MAR_ENGINE_GUI_LAYER_H
#define MAR_ENGINE_GUI_LAYER_H


#include "../../mar.h"
#include "../Layer.h"
#include "../../Editor/EditorManager.h"
#include "../../Editor/WidgetPanels/AllWidgetPanels.h"
#include "../../Editor/WidgetPanels/AllWidgetEvents.h"


namespace marengine {


	class EditorLayer : public Layer {
	public:

		EditorLayer() = default;
		EditorLayer(const char* name);

		void renderToViewport();

		// --- OVERRIDED METHODS --- // 

		void initialize() override;
		void update() override;
		void close() override;

	private:

		FAllWidgetPanels m_allWidgets;
		FAllWidgetEvents m_allEvents;
		FEditorManager m_editorManager;

	};


}


#endif // !MAR_ENGINE_GUI_LAYER_H
