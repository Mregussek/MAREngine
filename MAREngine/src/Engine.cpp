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


#include "Engine.h"
#include "Debug/Log.h"
#include "Core/scripting/PythonScript.h"
#include "LayerStack/layers/EntityLayer.h"
#include "LayerStack/layers/GUILayer.h"
#include "Core/graphics/Renderer/RenderPipeline.h"


namespace mar::engine {

		
	MAREngine* MAREngine::main_instance{ nullptr };


	void MAREngine::initialize() {
		main_instance = this;

		MAR_LOG_INIT();

		scripting::PythonScript::appendCurrentPath();

		m_editorName = m_pathLoad + " --- MAREngine";
	}

	void MAREngine::connectEntityLayerToGui(layers::LayerGUI* guilayer, layers::EntityLayer* entitylayer) {
		guilayer->submit(entitylayer->getSceneManager());
	}

	void MAREngine::resetStatistics() {
		graphics::RenderPipeline::getInstance().clearStatistics(); 
	}


}
