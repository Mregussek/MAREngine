/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H

#include "../../mar.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/Components.h"
#include "../../Core/ecs/ECS/Systems.h"
#include "../Filesystem/EditorFilesystem.h"
#include "../../Core/graphics/Mesh/MeshCreator.h"
#include "../../Window/Input.h"
#include "../EditorLogging.h"


namespace mar {
	namespace editor {


		struct GUI_EntityManagement {
			static ecs::Entity* currentEntity;
			static int32_t currentIndex;

			static void Scene_Entity_Modify(bool is_play_mode);
			static void Scene_Entity_Modify_PopUp();

			static void Scene_Handle_TagComponent(bool& window_focused);
			static void Scene_Handle_RenderableComponent(bool& window_focused);
			static void Scene_Handle_TransformComponent(bool& window_focused);
			static void Scene_Handle_ScriptComponent(bool& window_focused);
			static void Scene_Handle_CameraComponent(bool& window_focused);
			static void Scene_Handle_ColorComponent(bool& window_focused);
			static void Scene_Handle_Texture2DComponent(bool& window_focused);
			static void Scene_Handle_TextureCubemapComponent(bool& window_focused);
			static void Scene_Handle_LightComponent(bool& window_focused);

		};



} }


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
