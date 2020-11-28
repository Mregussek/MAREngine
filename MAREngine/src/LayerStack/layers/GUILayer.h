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


#ifndef MAR_ENGINE_GUI_LAYER_H
#define MAR_ENGINE_GUI_LAYER_H


#include "../../mar.h"
#include "../Layer.h"
#include "../../Editor/GUI.h"
#include "../../Editor/GUIPanels/GUI_Graphics.h"


namespace mar {
	namespace ecs { class SceneManager; }
}
namespace mar::layers {


	class LayerGUI : public Layer {
	public:

		LayerGUI() = default;
		LayerGUI(const char* name);

		void submit(ecs::SceneManager* manager);
		void renderToViewport();

		// --- OVERRIDED METHODS --- // 

		void initialize() override;
		void update() override;
		void closeLayer() override;

	private:

		editor::GUI m_gui;
		//editor::GUI_Graphics m_guiGraphics;

	};


}


#endif // !MAR_ENGINE_GUI_LAYER_H
