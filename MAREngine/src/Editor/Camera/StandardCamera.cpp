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


#include "StandardCamera.h"
#include "Camera.h"
#include "../../Window/Window.h"


namespace marengine {


	bool StandardCamera::processFrame(Camera* camera, float deltaTime) const {
        static bool firstMouse = true;
        static bool holdingRightMouseButton{ false };
        bool userPressedSth = false;

        const bool userPressedRightMouseButton{ Window::isMousePressed(MAR_MOUSE_BUTTON_2) };

        if (userPressedRightMouseButton) {
            if(!holdingRightMouseButton) { firstMouse = true; }

            const float velocity = [&camera, this, deltaTime]()->float {
                const float currentSpeed{ checkForSpeedUpdate(camera, deltaTime) };

                if (Window::isKeyPressed(MAR_KEY_LEFT_SHIFT)) {
                    return 135.f + currentSpeed;
                }

                return currentSpeed;
            }();

            if (Window::isKeyPressed(MAR_KEY_W)) {
                camera->m_position = camera->m_position + (camera->m_front * velocity);
                userPressedSth = true;
            }
            if (Window::isKeyPressed(MAR_KEY_S)) {
                camera->m_position = camera->m_position - (camera->m_front * velocity);
                userPressedSth = true;
            }
            if (Window::isKeyPressed(MAR_KEY_A)) {
                camera->m_position = camera->m_position - (camera->m_right * velocity);
                userPressedSth = true;
            }
            if (Window::isKeyPressed(MAR_KEY_D)) {
                camera->m_position = camera->m_position + (camera->m_right * velocity);
                userPressedSth = true;
            }

            if (processMousePosition(camera, firstMouse)) {
                firstMouse = false;
                userPressedSth = true;
            }
        }
        else {
            firstMouse = true;
        }

        holdingRightMouseButton = userPressedRightMouseButton;

        return userPressedSth;
	}

    float StandardCamera::checkForSpeedUpdate(Camera* camera, float deltaTime) const {
        auto& speed = camera->m_movementSpeed;
        speed += Window::getScrollY();

        if (speed < 1.0f) { speed = 1.0f; }
        if (speed > 90.f) { speed = 90.f; }

        const float velocity = speed * deltaTime;

        return velocity;
    }

    bool StandardCamera::processMousePosition(Camera* camera, bool firstMouse) const {
        static float lastX{ 0.f };
        static float lastY{ 0.f };

        const float xoffset = Window::getMousePositionX();
        const float yoffset = Window::getMousePositionY();

        if (lastX == xoffset && lastY == yoffset) { return false; }
        
        if (firstMouse) {
            lastX = xoffset;
            lastY = yoffset;
        }

        constexpr float mouseSensitivity{ 0.2f };

        const float x_position = (xoffset - lastX) * mouseSensitivity;
        const float y_position = (lastY - yoffset) * mouseSensitivity;

        lastX = xoffset;
        lastY = yoffset;

        camera->m_yaw += x_position;
        camera->m_pitch += y_position;

        constexpr bool constrainPaith{ true };
        if (constrainPaith) {
            if (camera->m_pitch > 89.0f) { camera->m_pitch = 89.0f; }
            if (camera->m_pitch < -89.f) { camera->m_pitch = -89.f; };
        }

        return true;
    }


}
