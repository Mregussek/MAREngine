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
#include "../Components/Components.h"



namespace marengine {


	template<typename EntityType, typename ComponentType>
	void addComponentIfContains(const EntityType& src, const EntityType& dst) {
		if (src.hasComponent<ComponentType>()) {
			dst.addComponent<ComponentType>(src.getComponent<ComponentType>());
		}
	}

	void EntityOperation::copyEntity(const Entity& src, const Entity& dst) {
		// Default Component Copy
		dst.replaceComponent<TagComponent>(src.getComponent<TagComponent>());
		dst.replaceComponent<TransformComponent>(src.getComponent<TransformComponent>());

		// User-Available Components Copy
		addComponentIfContains<Entity, RenderableComponent>(src, dst);
		addComponentIfContains<Entity, ColorComponent>(src, dst);
		addComponentIfContains<Entity, Texture2DComponent>(src, dst);
		addComponentIfContains<Entity, TextureCubemapComponent>(src, dst);
		addComponentIfContains<Entity, LightComponent>(src, dst);
		addComponentIfContains<Entity, CameraComponent>(src, dst);
		addComponentIfContains<Entity, PythonScriptComponent>(src, dst);
	}
		

}
