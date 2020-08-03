/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "../../src/mar.h"
// Log && Filesystem
#include "../../src/Debug/Log.h"
#include "../../src/Editor/GUI/GUI.h"
#include "../../src/Editor/Filesystem/EditorFilesystem.h"
#include "../../src/Editor/Camera/Camera.h"
// Layers	 
#include "../../src/LayerStack/LayerStack.h"
#include "../../src/LayerStack/layers/GUILayer.h"
#include "../../src/LayerStack/layers/EntityLayer.h"
// Rendering 
#include "../../src/Core/ecs/Scene.h"
#include "../../src/Core/graphics/Renderer/RendererEntity.h"
// Window && Input
#include "../../src/Window/Input.h"
#include "../../src/Window/Window.h"
// Engine	 
#include "../../src/Engine.h"