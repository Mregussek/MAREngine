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


#include "Filesystem_Saving.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"
#include "../../Core/ecs/Components/Components.h"


namespace marengine {


	void Filesystem_Saving::saveScene(std::ofstream& ss, Scene* scene) {
		const auto& back = scene->getBackground();
		
		ss << "\n#SceneStart\n";
		ss << "#Scene_Name " << scene->getName() << "\n";
		ss << "#Scene_Background " << back.x << " " << back.y << " " << back.z << "\n";

		for (const auto& entity : scene->getEntities()) {
			saveEntity(ss, entity);
		}

		for (const auto& collection : scene->getCollections()) {
			saveCollection(ss, collection);
		}

		ss << "\n#SceneEnd\n";
	}

	void Filesystem_Saving::saveCollection(std::ofstream& ss, const EntityCollection& collection) {
		const auto& tag = collection.getComponent<TagComponent>();

		ss << "\n#EntityCollectionStart\n";

		{ // Save OBJ file path, if user added it
			if (collection.hasComponent<CollectionRenderableComponent>()) {
				ss << "#CollectionRenderableComponent " << collection.getComponent<CollectionRenderableComponent>().id << "\n";
			}
		}
		
		{ // Save entities that are in that collection
			const auto& entities = collection.getEntities();
			std::for_each(entities.cbegin(), entities.cend(), [&ss](const Entity& entity) {
				saveEntity(ss, entity);
			});
		}

		{ // Save collection trasform component
			const auto& transform = collection.getComponent<TransformComponent>();
			ss << "\n#CollectionTransformComponent Begin\n";
			ss << "#center " << transform.center.x << " " << transform.center.y << " " << transform.center.z << "\n";
			ss << "#angles " << transform.angles.x << " " << transform.angles.y << " " << transform.angles.z << "\n";
			ss << "#scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z << "\n";
		}
		
		ss << "#CollectionTagComponent " << tag.tag << "\n";
		ss << "\n#EntityCollectionEnd\n";
	}

	void Filesystem_Saving::saveEntity(std::ofstream& ss, const Entity& entity) {
		ss << "\n#EntityStart\n";

		{ // Tag Save
			const auto& tag = entity.getComponent<TagComponent>();
			ss << "#TagComponent " << tag.tag << "\n";
		}
		
		{ // Transform Save
			const auto& transform = entity.getComponent<TransformComponent>();
			ss << "#TransformComponent Begin\n";
			ss << "#center " << transform.center.x << " " << transform.center.y << " " << transform.center.z << "\n";
			ss << "#angles " << transform.angles.x << " " << transform.angles.y << " " << transform.angles.z << "\n";
			ss << "#scale " << transform.scale.x << " " << transform.scale.y << " " << transform.scale.z << "\n";
		}
		
		if (entity.hasComponent<RenderableComponent>()) {
			const auto& renderable = entity.getComponent<RenderableComponent>();
			ss << "#RenderableComponent " << renderable.name << "\n";
		}

		if (entity.hasComponent<ColorComponent>()) {
			const auto& color = entity.getComponent<ColorComponent>();
			ss << "#ColorComponent " << color.texture.x << " " << color.texture.y << " " << color.texture.z << " " << color.texture.w << "\n";
		}
		else if (entity.hasComponent<Texture2DComponent>()) {
			const auto& tex = entity.getComponent<Texture2DComponent>();
			ss << "#Texture2DComponent " << tex.texture << "\n";
		}
		else if (entity.hasComponent<TextureCubemapComponent>()) {
			const auto& cube = entity.getComponent<TextureCubemapComponent>();
			ss << "#TextureCubemapComponent " << cube.texture << "\n";
		}

		if (entity.hasComponent<LightComponent>()) {
			const auto& light = entity.getComponent<LightComponent>();

			ss << "#LightComponent Begin\n";

			ss << "#ambientlight " << light.ambient.x << " " << light.ambient.y << " " << light.ambient.z << " " << light.ambient.w << "\n";
			ss << "#diffuselight " << light.diffuse.x << " " << light.diffuse.y << " " << light.diffuse.z << " " << light.diffuse.w << "\n";
			ss << "#specularlight " << light.specular.x << " " << light.specular.y << " " << light.specular.z << " " << light.specular.w << "\n";

			ss << "#constant " << light.constant << "\n";
			ss << "#linear " << light.linear << "\n";
			ss << "#quadratic " << light.quadratic << "\n";
			ss << "#shininess " << light.shininess << '\n';
		}

		if (entity.hasComponent<CameraComponent>()) {
			const auto& cam = entity.getComponent<CameraComponent>();

			ss << "#CameraComponent Begin\n";
			ss << "#id " << cam.id << "\n";

			if (cam.Perspective) { ss << "#used perspective\n"; }
			else { ss << "#used orthographic\n"; }

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

		if (entity.hasComponent<ScriptComponent>()) {
			const auto& script = entity.getComponent<ScriptComponent>();
			ss << "#ScriptComponent " << script.script << "\n";
		}

		ss << "\n#EntityEnd\n";
	}


}
