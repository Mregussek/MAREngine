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


#include "StandardCamera.h"
#include "Camera.h"
#include "../../Window/Window.h"


namespace mar::editor {


	bool StandardCamera::processFrame(float deltaTime) {
        static bool firstMouse = true;
        static bool holdingRightMouseButton{ false };
        bool userPressedSth = false;

        const bool userPressedRightMouseButton{ Window::isMousePressed(MAR_MOUSE_BUTTON_2) };

        if (userPressedRightMouseButton) {
            if(!holdingRightMouseButton) { firstMouse = true; }

            auto& position = Camera::Instance()->m_position;
            auto& front = Camera::Instance()->m_front;
            auto& right = Camera::Instance()->m_right;

            const float velocity = checkForSpeedUpdate(deltaTime);

            if (Window::isKeyPressed(MAR_KEY_W)) {
                position += front * velocity;
                userPressedSth = true;
            }
            if (Window::isKeyPressed(MAR_KEY_S)) {
                position -= front * velocity;
                userPressedSth = true;
            }
            if (Window::isKeyPressed(MAR_KEY_A)) {
                position -= right * velocity;
                userPressedSth = true;
            }
            if (Window::isKeyPressed(MAR_KEY_D)) {
                position += right * velocity;
                userPressedSth = true;
            }

            if (processMousePosition(firstMouse)) {
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

    float StandardCamera::checkForSpeedUpdate(float deltaTime) {
        auto& speed = Camera::Instance()->m_movementSpeed;
        speed += window::Window::getScrollY();

        if (speed < 1.0f) { speed = 1.0f; }
        if (speed > 90.f) { speed = 90.f; }

        const float velocity = speed * deltaTime;

        return velocity;
    }

    bool StandardCamera::processMousePosition(bool firstMouse) {
        static float lastX{ 0.f };
        static float lastY{ 0.f };

        const float xoffset = window::Window::getMousePositionX();
        const float yoffset = window::Window::getMousePositionY();

        if (lastX == xoffset && lastY == yoffset) { return false; }
        
        auto& speed = Camera::Instance()->m_movementSpeed;
        auto& yaw = Camera::Instance()->m_yaw;
        auto& pitch = Camera::Instance()->m_pitch;

        if (firstMouse) {
            lastX = xoffset;
            lastY = yoffset;
        }

        constexpr float mouseSensitivity{ 0.2f };

        const float x_position = (xoffset - lastX) * mouseSensitivity;
        const float y_position = (lastY - yoffset) * mouseSensitivity;

        lastX = xoffset;
        lastY = yoffset;

        yaw += x_position;
        pitch += y_position;

        constexpr bool constrainPaith{ true };
        if (constrainPaith) {
            if (pitch > 89.0f) { pitch = 89.0f; }
            if (pitch < -89.f) { pitch = -89.f; };
        }

        return true;
    }


}
