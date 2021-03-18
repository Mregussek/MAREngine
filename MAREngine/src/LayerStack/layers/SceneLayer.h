/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_ENTITY_LAYER_H
#define MAR_ENGINE_ENTITY_LAYER_H

#include <string>
#include "../Layer.h"
#include "../../Core/ecs/SceneManagerEditor.h"


namespace marengine {

	class Scene;


	class FSceneLayer : public ILayer {
	public:

		void create(const std::string& scenePath);

		void update() override;
		void close() override;

		FSceneManagerEditor* getSceneManager();

	private:

		FSceneManagerEditor m_sceneManagerEditor;

	};


}


#endif // !MAR_ENGINE_ENTITY_LAYER_H
