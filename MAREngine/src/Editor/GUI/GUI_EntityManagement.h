/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H

#include "../../mar.h"
#include "../../Core/ecs/ECS/Components.h"
#include "../../Core/ecs/ECS/Systems.h"
#include "../Filesystem/EditorFilesystem.h"
#include "../../Core/graphics/Mesh/MeshCreator.h"
#include "../../Window/Input.h"
#include "../../Core/ecs/SceneEvents.h"
#include "../EditorLogging.h"


namespace mar {
	namespace ecs { class Entity; }

	namespace editor {


		struct GUI_EntityManagement {
			static graphics::RenderCamera* render_cam;
			static ecs::Entity* currentEntity;
			static int32_t currentIndex;

			static void Scene_Entity_Modify(bool is_play_mode);

		private:
			static void Scene_Entity_Modify_PopUp();

			static void Scene_Handle_TagComponent(bool window_focused);
			static void Scene_Handle_RenderableComponent(bool window_focused);
			static void Scene_Handle_TransformComponent(bool window_focused, bool is_play_mode);
			static void Scene_Handle_ScriptComponent(bool window_focused);
			static void Scene_Handle_CameraComponent(bool window_focused, bool is_play_mode);
			static void Scene_Handle_ColorComponent(bool window_focused, bool is_play_mode);
			static void Scene_Handle_Texture2DComponent(bool window_focused);
			static void Scene_Handle_TextureCubemapComponent(bool window_focused);
			static void Scene_Handle_LightComponent(bool window_focused, bool is_play_mode);

			// --------------------------------------------
			// TEMPLATES TO WRITE MORE REUSABLE CODE
			// --------------------------------------------

			template<typename T>
			static bool Button_ChooseRenderable(ecs::RenderableComponent& renderable, const char* buttonName) {
				if (ImGui::Button(buttonName)) {
					renderable.id = T::getID();
					renderable.vertices = T::getVertices();
					renderable.indices = T::getIndices();

					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);

					return true;
				}

				return false;
			}

			static bool Button_LoadObj(ecs::RenderableComponent& renderable, const char* buttonName, const char* path) {
				if (ImGui::Button(buttonName)) {
					graphics::MeshCreator::OBJ::loadOBJ(path);

					renderable.id = graphics::MeshCreator::OBJ::getID();
					renderable.vertices = graphics::MeshCreator::OBJ::vertices;
					renderable.indices = graphics::MeshCreator::OBJ::indices;

					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);

					return true;
				}

				return false;
			}
		};



} }


#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_MANAGEMENT_H
