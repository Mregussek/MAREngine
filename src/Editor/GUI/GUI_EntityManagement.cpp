/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI_EntityManagement.h"
#include "../../Core/ecs/SceneEvents.h"
#include "GUI_TextEditor.h"


namespace mar {
	namespace editor {

		graphics::RenderCamera* GUI_EntityManagement::render_cam{ nullptr };
		ecs::Entity* GUI_EntityManagement::currentEntity{ nullptr };
		int32_t GUI_EntityManagement::currentIndex{ -1 };


		void GUI_EntityManagement::Scene_Entity_Modify(bool is_play_mode) {
			ImGui::Begin("Entity Modification");

			if (!currentEntity) {
				ImGui::Text("No entity selected!");
				ImGui::End();
				return;
			}

			static bool is_window_focused = false;

			if (ImGui::IsWindowFocused()) is_window_focused = true;
			else is_window_focused = false;

			if (is_play_mode) {
				ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

				if (currentEntity->hasComponent<ecs::TransformComponent>())
					Scene_Handle_TransformComponent(is_window_focused, is_play_mode);

				if (currentEntity->hasComponent<ecs::CameraComponent>())
					Scene_Handle_CameraComponent(is_window_focused, is_play_mode);

				if (currentEntity->hasComponent<ecs::ColorComponent>())
					Scene_Handle_ColorComponent(is_window_focused, is_play_mode);

				if (currentEntity->hasComponent<ecs::LightComponent>())
					Scene_Handle_LightComponent(is_window_focused, is_play_mode);

				ImGui::End();

				EDITOR_TRACE("GUI: scene_entity_modify end (PLAY MODE)");
				return;
			}

			if (currentEntity->hasComponent<ecs::TagComponent>())
				Scene_Handle_TagComponent(is_window_focused);

			if (currentEntity->hasComponent<ecs::TransformComponent>())
				Scene_Handle_TransformComponent(is_window_focused, is_play_mode);

			if (currentEntity->hasComponent<ecs::ScriptComponent>())
				Scene_Handle_ScriptComponent(is_window_focused);

			if (currentEntity->hasComponent<ecs::RenderableComponent>())
				Scene_Handle_RenderableComponent(is_window_focused);

			if (currentEntity->hasComponent<ecs::CameraComponent>())
				Scene_Handle_CameraComponent(is_window_focused, is_play_mode);

			if (currentEntity->hasComponent<ecs::ColorComponent>())
				Scene_Handle_ColorComponent(is_window_focused, is_play_mode);

			if (currentEntity->hasComponent<ecs::Texture2DComponent>())
				Scene_Handle_Texture2DComponent(is_window_focused);

			if (currentEntity->hasComponent<ecs::TextureCubemapComponent>())
				Scene_Handle_TextureCubemapComponent(is_window_focused);

			if (currentEntity->hasComponent<ecs::LightComponent>())
				Scene_Handle_LightComponent(is_window_focused, is_play_mode);

			Scene_Entity_Modify_PopUp();

			ImGui::End();

			EDITOR_TRACE("GUI: scene_entity_modify");
		}

		void GUI_EntityManagement::Scene_Entity_Modify_PopUp() {
			static bool b = false;

			// PopMenu should be opened?
			{
				if (ImGui::IsWindowFocused())
					b = window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_2);
				else
					b = false;

				if (b) {
					ImGui::OpenPopup("SceneEntityModifyPopUp");
					if (window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_1))
						b = false;
				}
			}

			// Actual PopUp menu
			{
				if (ImGui::BeginPopup("SceneEntityModifyPopUp")) {
					if (ImGui::BeginMenu("Add Component")) {
						if (!currentEntity->hasComponent<ecs::RenderableComponent>())
							if (ImGui::MenuItem("Add RenderableComponent"))
								currentEntity->addComponent<ecs::RenderableComponent>(ECS_RENDERABLE);

						static bool has_color_texture;
						has_color_texture = !currentEntity->hasComponent<ecs::ColorComponent>() 
							&& !currentEntity->hasComponent<ecs::Texture2DComponent>()
							&& !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

						if (has_color_texture) {
							if (ImGui::BeginMenu("Add Color/Texture")) {
								if (ImGui::MenuItem("Add Texture2DComponent")) {
									currentEntity->addComponent<ecs::Texture2DComponent>(ECS_TEXTURE2D);
									ecs::SceneEvents::Instance().updatedTexture2D(currentEntity, currentIndex);
								}
									
								if (ImGui::MenuItem("Add TextureCubemapComponent")) {
									currentEntity->addComponent<ecs::TextureCubemapComponent>(ECS_CUBEMAP);
									ecs::SceneEvents::Instance().updatedCubemap(currentEntity, currentIndex);
								}
									
								if (ImGui::MenuItem("Add ColorComponent")) {
									currentEntity->addComponent<ecs::ColorComponent>(ECS_COLOR);
									ecs::SceneEvents::Instance().updatedColor(currentEntity, currentIndex);
								}

								ImGui::EndMenu();
							}
						}

						if (!currentEntity->hasComponent<ecs::LightComponent>())
							if (ImGui::MenuItem("Add LightComponent")) {
								currentEntity->addComponent<ecs::LightComponent>(ECS_LIGHT);
								ecs::SceneEvents::Instance().updatedLight(currentEntity, currentIndex);
							}

						if (!currentEntity->hasComponent<ecs::CameraComponent>())
							if (ImGui::MenuItem("Add CameraComponent")) {
								currentEntity->addComponent<ecs::CameraComponent>(ECS_CAMERA);
								ecs::SceneEvents::Instance().updatedCamera(currentEntity, currentIndex);
							}

						if (!currentEntity->hasComponent<ecs::ScriptComponent>())
							if (ImGui::MenuItem("Add ScriptComponent")) {
								currentEntity->addComponent<ecs::ScriptComponent>(ECS_SCRIPT);
								ecs::SceneEvents::Instance().updatedScript(currentEntity, currentIndex);
							}

						ImGui::EndMenu();
					}

					ImGui::EndPopup();
				}
			}

			EDITOR_TRACE("GUI: scene_entity_modify_popup");
		}

		void GUI_EntityManagement::Scene_Handle_TagComponent(bool window_focused) {
			ImGui::Text("TagComponent\n");

			static char* input;
			auto& tag = currentEntity->getComponent<ecs::TagComponent>();

			input = (char*)tag.tag.c_str();

			if (ImGui::InputText("Tag", input, 25))
				tag.tag = std::string(input);

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling tag component");
		}

		void GUI_EntityManagement::Scene_Handle_TransformComponent(bool window_focused, bool is_play_mode) {
			ImGui::Separator();
			ImGui::Text("TransformComponent\n");

			auto& tran = currentEntity->getComponent<ecs::TransformComponent>();

			// Sliders
			{
				static bool updated_transform;
				updated_transform = false;

				static float last_general;
				last_general = tran.general_scale;

				if (ImGui::DragFloat3("Position", maths::vec3::value_ptr_nonconst(tran.center), 0.05f, -200.0f, 200.0f, "%.2f", 1.f) ) updated_transform = true;
				if (ImGui::DragFloat3("Rotation", maths::vec3::value_ptr_nonconst(tran.angles), 0.5f, -360.f, 360.f, "%.2f", 1.f)	 ) updated_transform = true;
				if (ImGui::DragFloat3("Scale", maths::vec3::value_ptr_nonconst(tran.scale), 0.01f, 0.f, 20.0f, "%.2f", 1.f)			 ) updated_transform = true;
				if (ImGui::DragFloat("GeneralScale", &tran.general_scale, 0.01f, 0.001f, 10.f, "%.3f", 1.f)							 ) updated_transform = true;

				if (last_general != tran.general_scale)
					tran.scale += tran.general_scale - last_general;

				if (ImGui::Button("Reset to default scale")) {
					tran.general_scale = 1.f;
					tran.scale.x = 1.f;
					tran.scale.y = 1.f;
					tran.scale.z = 1.f;
					updated_transform = true;
				}

				if (updated_transform) {
					ecs::System::handleTransformComponent(tran);
					if(!is_play_mode) ecs::SceneEvents::Instance().updateTransform(currentEntity, currentIndex);
				}
			}

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling transform component");
		}

		void GUI_EntityManagement::Scene_Handle_ScriptComponent(bool window_focused) {
			ImGui::Separator();
			ImGui::Text("ScriptComponent\n");
			ImGui::SameLine();
			if (ImGui::MenuItem("Remove Script")) {
				currentEntity->removeComponent<ecs::ScriptComponent>(ECS_SCRIPT);
				GUI_TextEditor::Instance().setEditorText("def main():\n\tpass\n");
				GUI_TextEditor::Instance().setEditorTitle("Empty");
				return;
			}

			auto& script = currentEntity->getComponent<ecs::ScriptComponent>();

			static char input[50];
			strcpy_s(input, script.script.c_str());
			if (ImGui::InputText(" - script", input, 50))
				script.script = std::string(input);

			if (ImGui::Button("Create new script")) 
				GUI_TextEditor::Instance().createNewFile(script.script);

			ImGui::SameLine();

			if (ImGui::Button("Load Script from file")) {
				script.source = Filesystem::loadPyScript(script.script.c_str());
				GUI_TextEditor::Instance().setEditorText(script.source);
				GUI_TextEditor::Instance().setEditorTitle(script.script.c_str());
			}
				
			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling script component");
		}

		void GUI_EntityManagement::Scene_Handle_RenderableComponent(bool window_focused) {
			static bool GUI_modify_renderable = false; // should display option to modify the whole RenderableComponent ?
			static bool GUI_display_obj = false; // should display obj file loading to RenderableComponent ?

			auto& renderable = currentEntity->getComponent<ecs::RenderableComponent>();

			ImGui::Separator();
			ImGui::Text("RenderableComponent\n");
			ImGui::SameLine();

			static bool color_texture_OR;
			color_texture_OR = !currentEntity->hasComponent<ecs::ColorComponent>() 
				|| !currentEntity->hasComponent<ecs::Texture2DComponent>()
				|| !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

			if (color_texture_OR)
				if (ImGui::MenuItem("Remove Renderable")) {
					currentEntity->removeComponent<ecs::RenderableComponent>(ECS_RENDERABLE);
					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);
					return;
				}

			ImGui::Text(renderable.id.c_str());

			static bool color_texture_AND;
			color_texture_AND = !currentEntity->hasComponent<ecs::ColorComponent>() 
				&& !currentEntity->hasComponent<ecs::Texture2DComponent>()
				&& !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

			if (color_texture_AND)
				ImGui::Text("WARNING: Object will not be rendered until you will add Color or Texture!");

			if (renderable.vertices.empty())
				GUI_modify_renderable = true;

			if (GUI_modify_renderable) {
				if (ImGui::Button("Cube")) {
					renderable.id = "Cube";
					renderable.vertices = graphics::MeshCreator::Cube::getVertices();
					renderable.indices = graphics::MeshCreator::Cube::getIndices();
					GUI_modify_renderable = false;
					GUI_display_obj = false;

					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);
				}

				ImGui::SameLine();

				if (ImGui::Button("Pyramid")) {
					renderable.id = "Pyramid";
					renderable.vertices = graphics::MeshCreator::Pyramid::getVertices();
					renderable.indices = graphics::MeshCreator::Pyramid::getIndices();
					GUI_modify_renderable = false;
					GUI_display_obj = false;

					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);
				}

				ImGui::SameLine();

				if (ImGui::Button("Wall")) {
					renderable.id = "Wall";
					renderable.vertices = graphics::MeshCreator::Wall::getVertices();
					renderable.indices = graphics::MeshCreator::Wall::getIndices();
					GUI_modify_renderable = false;
					GUI_display_obj = false;

					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);
				}

				ImGui::SameLine();

				if (ImGui::Button("Surface")) {
					renderable.id = "Surface";
					renderable.vertices = graphics::MeshCreator::Surface::getVertices();
					renderable.indices = graphics::MeshCreator::Surface::getIndices();
					GUI_modify_renderable = false;
					GUI_display_obj = false;

					ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);
				}

				ImGui::SameLine();

				if (ImGui::Button("Load OBJ"))
					GUI_display_obj = true;

				if (GUI_display_obj) {
					static char filename[30]{ "empty" };
					ImGui::InputText(".obj", filename, 30);
					static std::string load;
					load = "resources/objects/" + std::string(filename) + ".obj";

					ImGui::Text(".obj file, which will be loaded: ");
					ImGui::SameLine();
					ImGui::Text(load.c_str());
					ImGui::Text("WARNING: if .obj file do not exist, no renderable will be assigned!");

					if (ImGui::Button("Load")) {
						graphics::MeshCreator::OBJ::loadOBJ(load.c_str());

						renderable.id = graphics::MeshCreator::OBJ::vertices.empty() ? "empty" : load;
						renderable.vertices = graphics::MeshCreator::OBJ::vertices;
						renderable.indices = graphics::MeshCreator::OBJ::indices;
						GUI_modify_renderable = false;
						GUI_display_obj = false;

						ecs::SceneEvents::Instance().updateRenderables(currentEntity, currentIndex);
					}

					ImGui::SameLine();
				}

				if (ImGui::Button("Do not modify")) {
					GUI_display_obj = false;
					GUI_modify_renderable = false;
				}
			}
			else {
				if (ImGui::Button("ModifyRenderable"))
					GUI_modify_renderable = true;
			}

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling renderable component");
		}

		void GUI_EntityManagement::Scene_Handle_CameraComponent(bool window_focused, bool is_play_mode) {
			static char* GUI_input{ (char*)"empty" };
			auto& camcmp = currentEntity->getComponent<ecs::CameraComponent>();

			ImGui::Separator();
			ImGui::Text("CameraComponent\n");
			ImGui::SameLine();
			if (camcmp.id.find("main") == std::string::npos) {
				if (ImGui::Button("Remove Camera")) {
					currentEntity->removeComponent<ecs::CameraComponent>(ECS_CAMERA);
					return;
				}
			}

			ImGui::Text("WARNING: If you want to use this camera, make sure\nthat it is the only with \"main\" CameraID!");

			GUI_input = (char*)camcmp.id.c_str();
			if (ImGui::InputText("CameraID", GUI_input, 30))
				camcmp.id = std::string(GUI_input);

			ImGui::Checkbox("Perspective (TRUE) / Orthographic (FALSE)", &camcmp.Perspective);

			static bool updated_camera;
			updated_camera = false;

			if (camcmp.Perspective) {
				if (ImGui::DragFloat("AspectRatio", &camcmp.p_aspectRatio, 0.1f, 1.f, 10.f)) updated_camera = true;
				if (ImGui::Button("Set 16:9")) camcmp.p_aspectRatio = 16.f / 9.f;
				ImGui::SameLine();
				if (ImGui::Button("Set 8:5")) camcmp.p_aspectRatio = 8.f / 5.f;
				ImGui::SameLine();
				if (ImGui::Button("Set 4:3")) camcmp.p_aspectRatio = 4.f / 3.f;

				if (ImGui::DragFloat("Field Of View", &camcmp.p_fov, 0.1f, 1.f, 90.f) ) updated_camera = true;
				if (ImGui::DragFloat("Near", &camcmp.p_near, 0.01f, 0.001f, 150.f)	  ) updated_camera = true;
				if (ImGui::DragFloat("Far", &camcmp.p_far, 0.1f, 0.001f, 150.f)		  ) updated_camera = true;
			}
			else {
				if (ImGui::DragFloat("Left", &camcmp.o_left, 0.1f, -100.f, 100.f)		) updated_camera = true;
				if (ImGui::DragFloat("Right", &camcmp.o_right, 0.1f, -100.f, 100.f)		) updated_camera = true;
				if (ImGui::DragFloat("Top", &camcmp.o_top, 0.1f, -100.f, 100.f)			) updated_camera = true;
				if (ImGui::DragFloat("Bottom", &camcmp.o_bottom, 0.1f, -100.f, 100.f)	) updated_camera = true;
				if (ImGui::DragFloat("Near", &camcmp.o_near, 0.1f, 0.001f, 150.f)		) updated_camera = true;
				if (ImGui::DragFloat("Far", &camcmp.o_far, 0.1f, 0.001f, 150.f)			) updated_camera = true;
			}

			if (updated_camera)
				if(!is_play_mode) ecs::SceneEvents::Instance().updatedCamera(currentEntity, currentIndex);

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling camera component");
		}

		void GUI_EntityManagement::Scene_Handle_ColorComponent(bool window_focused, bool is_play_mode) {
			ImGui::Separator();
			ImGui::Text("ColorComponent\n");
			ImGui::SameLine();
			if (ImGui::MenuItem("Remove Color")) {
				currentEntity->removeComponent<ecs::ColorComponent>(ECS_COLOR);
				ecs::SceneEvents::Instance().updatedColor(currentEntity, currentIndex);
				return;
			}

			auto& color = currentEntity->getComponent<ecs::ColorComponent>();

			if (ImGui::ColorEdit3("- color", maths::vec3::value_ptr_nonconst(color.texture)))
				if(!is_play_mode) ecs::SceneEvents::Instance().updatedColor(currentEntity, currentIndex);

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling color component");
		}

		void GUI_EntityManagement::Scene_Handle_Texture2DComponent(bool window_focused) {
			static bool GUI_load_new_texture = false;
			auto& tex = currentEntity->getComponent<ecs::Texture2DComponent>();

			ImGui::Separator();
			ImGui::Text("Texture2DComponent\n");
			ImGui::SameLine();
			if (ImGui::MenuItem("Remove Texture")) {
				currentEntity->removeComponent<ecs::Texture2DComponent>(ECS_TEXTURE2D);
				ecs::SceneEvents::Instance().updatedTexture2D(currentEntity, currentIndex);
				return;
			}

			ImGui::Text("Current Texture: ");
			ImGui::SameLine();
			ImGui::Text(tex.texture.c_str());

			if (tex.texture != "empty") {
				if (!GUI_load_new_texture) {
					if (ImGui::Button("Load new Texture menu"))
						GUI_load_new_texture = true;
				}
				else {
					if (ImGui::Button("Close Texture Menu"))
						GUI_load_new_texture = false;
				}
			}
			else {
				GUI_load_new_texture = true;
			}

			if (!GUI_load_new_texture) {
				EDITOR_TRACE("GUI: SELECTED-ENTITY: handling texture2D component (without loading new tex)");
				return;
			}

			static char input[50];
			strcpy_s(input, tex.texture.c_str());
			if (ImGui::InputText(" ex. .jpg / .png", input, 50))
				tex.texture = std::string(input);

			ImGui::Text("Texture, which will be loaded: ");
			ImGui::SameLine();
			ImGui::Text(tex.texture.c_str());
			ImGui::Text("WARNING: if texture do not exist, no shape will appear!");

			if (ImGui::Button("Load Texture")) {
				ecs::SceneEvents::Instance().updatedTexture2D(currentEntity, currentIndex);
				GUI_load_new_texture = false;
			}

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling texture2D component");
		}

		void GUI_EntityManagement::Scene_Handle_TextureCubemapComponent(bool window_focused) {
			static bool GUI_load_new_cubemap = false;
			auto& cubemap = currentEntity->getComponent<ecs::TextureCubemapComponent>();

			ImGui::Separator();
			ImGui::Text("CubemapComponent\n");
			ImGui::SameLine();
			if (ImGui::MenuItem("Remove Texture")) {
				currentEntity->removeComponent<ecs::TextureCubemapComponent>(ECS_CUBEMAP);
				ecs::SceneEvents::Instance().updatedCubemap(currentEntity, currentIndex);
				return;
			}

			ImGui::Text("Current Cubemap: ");
			ImGui::SameLine();
			ImGui::Text(cubemap.texture.c_str());

			if (cubemap.texture != "empty") {
				if (!GUI_load_new_cubemap) {
					if (ImGui::Button("Load new Texture menu"))
						GUI_load_new_cubemap = true;
				}
				else {
					if (ImGui::Button("Close Texture Menu"))
						GUI_load_new_cubemap = false;
				}
			}
			else {
				GUI_load_new_cubemap = true;
			}

			if (!GUI_load_new_cubemap) {
				EDITOR_TRACE("GUI: SELECTED-ENTITY: handling TextureCubemap component (without loading new tex)");
				return;
			}

			static char input[50];
			strcpy_s(input, cubemap.texture.c_str());
			if (ImGui::InputText(" ex. .jpg / .png", input, 50))
				cubemap.texture = std::string(input);

			ImGui::Text("Cubemap, which will be loaded: ");
			ImGui::SameLine();
			ImGui::Text(cubemap.texture.c_str());
			ImGui::Text("WARNING: if cubemap do not exist, no shape will appear!");

			if (ImGui::Button("Load Cubemap")) {
				ecs::SceneEvents::Instance().updatedCubemap(currentEntity, currentIndex);
				GUI_load_new_cubemap = false;
			}

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling TextureCubemap component");
		}

		void GUI_EntityManagement::Scene_Handle_LightComponent(bool window_focused, bool is_play_mode) {
			ImGui::Separator();
			ImGui::Text("LightComponent\n");

			ImGui::SameLine();

			if (ImGui::MenuItem("Remove Light")) {
				currentEntity->removeComponent<ecs::LightComponent>(ECS_LIGHT);
				ecs::SceneEvents::Instance().updatedLight(currentEntity, currentIndex);
				return;
			}

			auto& light = currentEntity->getComponent<ecs::LightComponent>();

			static bool updated_light;
			updated_light = false;

			if (ImGui::DragFloat3("Ambient Light", &light.ambient.x, 0.01f, 0.f, 1.f)	) updated_light = true;
			if (ImGui::DragFloat3("Diffuse Light", &light.diffuse.x, 0.01f, 0.f, 1.f)	) updated_light = true;
			if (ImGui::DragFloat3("Specular Light", &light.specular.x, 0.01f, 0.f, 1.f)	) updated_light = true;
																					
			if (ImGui::DragFloat("Constant", &light.constant, 0.001f, 0.f, 2.f)			) updated_light = true;
			if (ImGui::DragFloat("Linear", &light.linear, 0.001f, 0.f, 0.5f)			) updated_light = true;
			if (ImGui::DragFloat("Quadratic", &light.quadratic, 0.001f, 0.f, 0.1f)		) updated_light = true;
			if (ImGui::DragFloat("Shininess", &light.shininess, 0.5f, 0.f, 256.f)		) updated_light = true;

			if(updated_light)
				if(!is_play_mode) ecs::SceneEvents::Instance().updatedLight(currentEntity, currentIndex);

			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling light component");
		}


} }