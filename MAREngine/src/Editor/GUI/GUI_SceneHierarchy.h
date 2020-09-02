/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_SCENE_HIERARCHY_PANEL_H
#define MAR_ENGINE_GUI_SCENE_HIERARCHY_PANEL_H


#include "../../mar.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/SceneEvents.h"
#include "../../Core/ecs/SceneManager.h"



namespace mar {
	namespace editor {


		class GUI_SceneHierarchy {
		public:

			static void Scene_Hierarchy(ecs::SceneManager* manager);
			static void Scene_Hierarchy_PopUp(ecs::SceneManager* manager);

			static void Scene_Statistics();
		};


} }


#endif // !MAR_ENGINE_GUI_SCENE_HIERARCHY_PANEL_H