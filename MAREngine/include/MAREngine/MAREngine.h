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


// MAR 
#include "../../src/mar.h"

// Log
#include "../../src/Debug/Log.h"

// Editor
#include "../../src/Editor/GUI/GUI.h"
#include "../../src/Editor/ProjectSelectionGUI/ProjectSelectionGUI.h"
#include "../../src/Editor/Filesystem/EditorFilesystem.h"
#include "../../src/Editor/Camera/Camera.h"

// Layers	 
#include "../../src/LayerStack/LayerStack.h"
#include "../../src/LayerStack/layers/GUILayer.h"
#include "../../src/LayerStack/layers/EntityLayer.h"

// Entity Component System
#include "../../src/Core/ecs/Scene.h"
#include "../../src/Core/ecs/Entity/Entity.h"
#include "../../src/Core/ecs/Entity/EntityCollection.h"
#include "../../src/Core/ecs/Components/Components.h"

// Rendering 
#include "../../src/Core/graphics/Renderer/RenderPipeline.h"
#include "../../src/Core/graphics/Renderer/RendererBatch.h"

// Scripting
#include "../../src/Core/scripting/PythonScript.h"

// Window && Input
#include "../../src/Window/Input.h"
#include "../../src/Window/Window.h"

// Engine	 
#include "../../src/Engine.h"