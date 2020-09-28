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


namespace mar {
	namespace ecs {


		void EntityOperation::copyCollection(EntityCollection* src, EntityCollection* dst) {
			copyComponent<TagComponent>(src, dst);
			copyComponent<TransformComponent>(src, dst);
			if (src->hasComponent<CollectionRenderableComponent>()) {
				copyComponent<CollectionRenderableComponent>(src, dst);
			}

			for (size_t i = 0; i < src->getEntitiesCount(); i++) {
				auto& entity = src->getEntity(i);
				auto& dst_entity = dst->createEntity();
				copyEntity(&entity, &dst_entity);
			}
		}

		void EntityOperation::copyEntity(Entity* src, Entity* dst) {
			auto& components = src->getComponent<Components>();

			for (auto component : components.components) {
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
				case ECS_DEFAULT:
					copyComponent<Components>(component, src, dst);
					break;
				}
			}
		}

		template<typename T>
		void EntityOperation::copyComponent(EntityComponents entcmp, Entity* src, Entity* dst) {
			T com = src->getComponent<T>();
			T& dst_com = dst->hasComponent<T>() ? dst->getComponent<T>() : dst->addComponent<T>(entcmp);

			dst_com = com;
		}

		template<typename T>
		void EntityOperation::copyComponent(EntityCollection* src, EntityCollection* dst) {
			T com = src->getComponent<T>();
			T& dst_com = dst->hasComponent<T>() ? dst->getComponent<T>() : dst->addComponent<T>();

			dst_com = com;
		}
		

} }
