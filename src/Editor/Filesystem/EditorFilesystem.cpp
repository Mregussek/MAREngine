/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EditorFilesystem.h"


namespace mar {
	namespace editor {


		void Filesystem::saveToFile(ecs::Scene* scene, const char* filename) {
			std::ofstream ss(filename, std::ios::out | std::ios::trunc);

			if (!ss.is_open()) {
				MAR_CORE_ERROR("Cannot open file and save scene!");
				return;
			}

			ss << "MAREngine scene file\n";
			ss << "Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>\n\n";

			ss << "#Scene\n";
			ss << "#Scene_Name " << scene->getName() << "\n";

			for (auto& entity : scene->entities) {
				ss << "#Entity\n";

				for(int32_t i = 0; i < entity.components.size(); i++) {

					if (entity.components[i] == ECS_TAG) {
						auto& tag = entity.getComponent<ecs::TagComponent>();
						ss << "#TagComponent " << tag.tag << "\n";
					}
					else if (entity.components[i] == ECS_RENDERABLE) {
						auto& renderable = entity.getComponent<ecs::RenderableComponent>();
						ss << "#RenderableComponent " << renderable.id << "\n";
					}
					else if (entity.components[i] == ECS_TRANSFORM) {
						auto& transform = entity.getComponent<ecs::TransformComponent>();
						ss << "#TransformComponent Begin\n";
						ss << "#center " << transform.center.x << " " << transform.center.y << " " << transform.center.z << "\n";
						ss << "#angles " << transform.angles.x << " " << transform.angles.y << " " << transform.angles.z << "\n";
						ss << "#scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z << "\n";
						ss << "#general_scale " << transform.general_scale << "\n";
					}
					else if (entity.components[i] == ECS_COLOR) {
						auto& color = entity.getComponent<ecs::ColorComponent>();
						ss << "#ColorComponent " << color.color.x << " " << color.color.y << " " << color.color.z << "\n";
					}
					else if (entity.components[i] == ECS_TEXTURE2D) {
						auto& tex = entity.getComponent<ecs::Texture2DComponent>();
						ss << "#Texture2DComponent " << tex.texture << "\n";
					}
					else if (entity.components[i] == ECS_CUBEMAP) {
						auto& cube = entity.getComponent<ecs::TextureCubemapComponent>();
						ss << "#TextureCubemapComponent " << cube.cubemap << "\n";
					}
					else if (entity.components[i] == ECS_LIGHT) {
						auto& light = entity.getComponent<ecs::LightComponent>();

						ss << "#LightComponent Begin\n";

						ss << "#ambientlight " << light.ambient.x << " " << light.ambient.y << " " << light.ambient.z << "\n";
						ss << "#diffuselight " << light.diffuse.x << " " << light.diffuse.y << " " << light.diffuse.z << "\n";
						ss << "#specularlight " << light.specular.x << " " << light.specular.y << " " << light.specular.z << "\n";

						ss << "#ambientstrength " << light.ambientStrength.x << " " << light.ambientStrength.y << " " << light.ambientStrength.z << "\n";
						ss << "#diffusestrength " << light.diffuseStrength.x << " " << light.diffuseStrength.y << " " << light.diffuseStrength.z << "\n";
						ss << "#specularstrength " << light.specularStrength.x << " " << light.specularStrength.y << " " << light.specularStrength.z << "\n";
					
						ss << "#constant " << light.constant << "\n";
						ss << "#linear " << light.linear << "\n";
						ss << "#quadratic " << light.quadratic << "\n";
						ss << "#shininess " << light.shininess << '\n';
					}

 				}
			}
		}

		ecs::Scene* Filesystem::openFile(const char* filename) {

			return nullptr;
		}


} }