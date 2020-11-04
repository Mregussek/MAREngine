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
		dst.replaceComponent<TagComponent>(src);
		dst.replaceComponent<TransformComponent>(src);

		if (src.hasComponent<CollectionRenderableComponent>()) {
			dst.replaceComponent<CollectionRenderableComponent>(src);
		}

		const auto& entities = src.getEntities();

		std::for_each(entities.cbegin(), entities.cend(), [&dst](const Entity& entity) {
			const auto& entityDst = dst.createEntity();
			copyEntity(entity, entityDst);
		});
	}

	void EntityOperation::copyEntity(const Entity& src, const Entity& dst) {
		{ // Default Copy
			dst.replaceComponent<TagComponent>(src);
			dst.replaceComponent<TransformComponent>(src);
			dst.copyDefault(src);
		}

		if (src.hasComponent<RenderableComponent>()) {
			dst.replaceComponent<RenderableComponent>(src);
		}

		if (src.hasComponent<ColorComponent>()) {
			dst.replaceComponent<ColorComponent>(src);
		}
		else if (src.hasComponent<Texture2DComponent>()) {
			dst.replaceComponent<Texture2DComponent>(src);
		}
		else if (src.hasComponent<TextureCubemapComponent>()) {
			dst.replaceComponent<TextureCubemapComponent>(src);
		}

		if (src.hasComponent<LightComponent>()) {
			dst.replaceComponent<LightComponent>(src);
		}

		if (src.hasComponent<CameraComponent>()) {
			dst.replaceComponent<CameraComponent>(src);
		}

		if (src.hasComponent<ScriptComponent>()) {
			dst.replaceComponent<ScriptComponent>(src);
		}
	}
		

}
