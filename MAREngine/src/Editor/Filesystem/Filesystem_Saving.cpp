/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "Filesystem_Saving.h"


namespace mar {
	namespace editor {


		void Filesystem_Saving::saveScene(std::ofstream& ss, ecs::Scene* scene) {
			auto& back = scene->getBackground();
			
			ss << "\n#SceneStart\n";
			ss << "#Scene_Name " << scene->getName() << "\n";
			ss << "#Scene_Background " << back.x << " " << back.y << " " << back.z << "\n";

			for (auto& entity : scene->getEntities()) {
				Filesystem_Saving::saveEntity(ss, entity);
			}

			for (auto& collection : scene->getCollections()) {
				Filesystem_Saving::saveCollection(ss, collection);
			}

			ss << "\n#SceneEnd\n";
		}

		void Filesystem_Saving::saveCollection(std::ofstream& ss, const ecs::EntityCollection& collection) {
			auto& tag = collection.getComponent<ecs::TagComponent>();

			ss << "\n#EntityCollectionStart\n";
			ss << "#CollectionTagComponent " << tag.tag << "\n";

			for (auto& entity : collection.getEntities()) {
				saveEntity(ss, entity);
			}

			ss << "\n#EntityCollectionEnd\n";
		}

		void Filesystem_Saving::saveEntity(std::ofstream& ss, const ecs::Entity& entity) {
			ss << "\n#EntityStart\n";

			auto& com = entity.getComponent<ecs::Components>();

			for (auto& component : com.components) {

				if (component == ECS_TAG) {
					auto& tag = entity.getComponent<ecs::TagComponent>();
					ss << "#TagComponent " << tag.tag << "\n";
				}
				else if (component == ECS_RENDERABLE) {
					auto& renderable = entity.getComponent<ecs::RenderableComponent>();
					ss << "#RenderableComponent " << renderable.id << "\n";
				}
				else if (component == ECS_TRANSFORM) {
					auto& transform = entity.getComponent<ecs::TransformComponent>();
					ss << "#TransformComponent Begin\n";
					ss << "#center " << transform.center.x << " " << transform.center.y << " " << transform.center.z << "\n";
					ss << "#angles " << transform.angles.x << " " << transform.angles.y << " " << transform.angles.z << "\n";
					ss << "#scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z << "\n";
					ss << "#general_scale " << transform.general_scale << "\n";
				}
				else if (component == ECS_COLOR) {
					auto& color = entity.getComponent<ecs::ColorComponent>();
					ss << "#ColorComponent " << color.texture.x << " " << color.texture.y << " " << color.texture.z << "\n";
				}
				else if (component == ECS_TEXTURE2D) {
					auto& tex = entity.getComponent<ecs::Texture2DComponent>();
					ss << "#Texture2DComponent " << tex.texture << "\n";
				}
				else if (component == ECS_CUBEMAP) {
					auto& cube = entity.getComponent<ecs::TextureCubemapComponent>();
					ss << "#TextureCubemapComponent " << cube.texture << "\n";
				}
				else if (component == ECS_LIGHT) {
					auto& light = entity.getComponent<ecs::LightComponent>();

					ss << "#LightComponent Begin\n";

					ss << "#ambientlight " << light.ambient.x << " " << light.ambient.y << " " << light.ambient.z << "\n";
					ss << "#diffuselight " << light.diffuse.x << " " << light.diffuse.y << " " << light.diffuse.z << "\n";
					ss << "#specularlight " << light.specular.x << " " << light.specular.y << " " << light.specular.z << "\n";

					ss << "#constant " << light.constant << "\n";
					ss << "#linear " << light.linear << "\n";
					ss << "#quadratic " << light.quadratic << "\n";
					ss << "#shininess " << light.shininess << '\n';
				}
				else if (component == ECS_CAMERA) {
					auto& cam = entity.getComponent<ecs::CameraComponent>();

					ss << "#CameraComponent Begin\n";
					ss << "#id " << cam.id << "\n";

					if (cam.Perspective)
						ss << "#used perspective\n";
					else
						ss << "#used orthographic\n";

					ss << "#type perspective\n";
					ss << "#fov " << cam.p_fov << "\n";
					ss << "#aspectratio " << cam.p_aspectRatio << "\n";
					ss << "#near " << cam.p_near << "\n";
					ss << "#far " << cam.p_far << "\n";

					ss << "#type orthographic\n";
					ss << "#left " << cam.o_left << "\n";
					ss << "#right " << cam.o_right << "\n";
					ss << "#top " << cam.o_top << "\n";
					ss << "#bottom " << cam.o_bottom << "\n";
					ss << "#near " << cam.o_far << "\n";
					ss << "#far " << cam.o_far << "\n";
				}
				else if (component == ECS_SCRIPT) {
					auto& script = entity.getComponent<ecs::ScriptComponent>();

					ss << "#ScriptComponent " << script.script << "\n";
				}
			}

			ss << "\n#EntityEnd\n";
		}


} }
