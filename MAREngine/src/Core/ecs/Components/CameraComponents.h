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


#ifndef MAR_ENGINE_CAMERA_COMPONENTS_H
#define MAR_ENGINE_CAMERA_COMPONENTS_H


#include "../../../mar.h"
#include "../../graphics/RenderAPI/RenderCamera.h"


namespace marengine {


	/**
	 * @struct CameraComponent CameraComponents.h "Core/ecs/Components/CameraComponents.h"
	 * @brief component with RenderCamera instance and parameters, with which we can choose how to create Projection Matrix.
	 * 
	 * p_ prefix stands for perspective, o_ for orthographic.
	 * RenderCamera can be pushed to rendering with FEventsCameraEntity::onGameCameraSet()
	 * only if it is main renderCamera for scene. You can check if it is main camera by using isMainCamera() method.
	 */
	struct CameraComponent {

		CameraComponent() = default;
		CameraComponent(const CameraComponent & cam) = default;

		/**
		 * @brief Method checks, if renderCamera is a main one for current scene and returns result.
		 * @return returns true, if renderCamera instance is main camera for current scene.
		 */
		MAR_NO_DISCARD bool isMainCamera() const;

		RenderCamera renderCamera;
		std::string id{ "secondary" };   // must contain "main" to be main camera
		
		float p_fov{ 45.f };
		float p_aspectRatio{ 4.f / 3.f };
		float p_near{ 0.01f };
		float p_far{ 100.0f };

		float o_left{ -10.f };
		float o_right{ 10.f };
		float o_top{ 10.f };
		float o_bottom{ -10.f };
		float o_near{ 0.01f };
		float o_far{ 100.0f };

		bool Perspective{ true }; // true - perspective / false - orthographic

	};


}


#endif // !MAR_ENGINE_CAMERA_COMPONENTS_H
