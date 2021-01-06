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


<<<<<<< HEAD:MAREngine/src/Core/graphics/Renderer/RendererEvents.cpp
#include "RendererEvents.h"
#include "../RenderAPI/RenderStatistics.h"
=======
#ifndef MAR_ENGINE_F_EVENTS_LIGHT_BATCH_H
#define MAR_ENGINE_F_EVENTS_LIGHT_BATCH_H
>>>>>>> renderissue:MAREngine/src/Core/graphics/Lightning/EventsLightBatch.h


namespace marengine {

	class Entity;

<<<<<<< HEAD:MAREngine/src/Core/graphics/Renderer/RendererEvents.cpp
	void FRendererEvents::onDrawCall() {

		RenderStatistics::Instance->drawCallsCount++;

	}
=======

	class FEventsLightBatch {
	public:

		static void onPointLightUpdate(const Entity& entity);
		static void onPointLightPositionUpdate(const Entity& entity);
>>>>>>> renderissue:MAREngine/src/Core/graphics/Lightning/EventsLightBatch.h

	};

<<<<<<< HEAD:MAREngine/src/Core/graphics/Renderer/RendererEvents.cpp
}
=======
}


#endif // !MAR_ENGINE_F_EVENTS_LIGHT_BATCH_H
>>>>>>> renderissue:MAREngine/src/Core/graphics/Lightning/EventsLightBatch.h
