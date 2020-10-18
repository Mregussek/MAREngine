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


#include "EntityOperation.h"
#include "Entity.h"
#include "EntityCollection.h"


namespace mar::ecs {


	void EntityOperation::copyCollection(const EntityCollection& src, const EntityCollection& dst) {
		copyComponent<TagComponent>(src, dst);
		copyComponent<TransformComponent>(src, dst);

		if (src.hasComponent<CollectionRenderableComponent>()) { 
			copyComponent<CollectionRenderableComponent>(src, dst);
		}

		const auto& entities = src.getEntities();

		std::for_each(entities.cbegin(), entities.cend(), [&dst](const Entity& entity) {
			const auto& entityDst = dst.createEntity();
			copyEntity(entity, entityDst);
		});
	}

	void EntityOperation::copyEntity(const Entity& src, const Entity& dst) {
		const auto& components = src.getComponent<Components>();

		for (const auto component : components.components) {
			switch (component) {
			case ECS_RENDERABLE:
				copyComponent<RenderableComponent>(component, src, dst);
				break;
			case ECS_COLOR:
				copyComponent<ColorComponent>(component, src, dst);
				break;
			case ECS_TEXTURE2D:
				copyComponent<Texture2DComponent>(component, src, dst);
				break;
			case ECS_CUBEMAP:
				copyComponent<TextureCubemapComponent>(component, src, dst);
				break;
			case ECS_LIGHT:
				copyComponent<LightComponent>(component, src, dst);
				break;
			case ECS_CAMERA:
				copyComponent<CameraComponent>(component, src, dst);
				break;
			case ECS_SCRIPT:
				copyComponent<ScriptComponent>(component, src, dst);
				break;
			case ECS_TRANSFORM:
				copyComponent<TransformComponent>(component, src, dst);
				break;
			case ECS_TAG:
				copyComponent<TagComponent>(component, src, dst);
				break;
			}
		}

		src.copyDefault(dst);
	}

	template<typename T>
	void EntityOperation::copyComponent(EntityComponents entcmp, const Entity& src, const Entity& dst) {
		if (dst.hasComponent<T>()) { dst.replaceComponent<T>(src); }
		else {
			const auto com = src.getComponent<T>();
			auto& component = dst.addComponent<T>(entcmp);
			component = com;
		}
	}

	template<typename T>
	void EntityOperation::copyComponent(const EntityCollection& src, const EntityCollection& dst) {
		if (dst.hasComponent<T>()) { dst.replaceComponent<T>(src); }
		else {
			const auto com = src.getComponent<T>();
			auto& component = dst.addComponent<T>();
			component = com;
		}
	}
		

}
