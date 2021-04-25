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


#include "../public/Camera.h"
#include "../../../Window/Window.h"


namespace marengine {


    static bool processQuatAngles(const FWindow* pWindow, float& yaw, float& pitch,
                                  bool firstMouse) {
        static float lastX{ 0.f };
        static float lastY{ 0.f };

        const float xoffset = pWindow->getMousePositionX();
        const float yoffset = pWindow->getMousePositionY();

        if (lastX == xoffset && lastY == yoffset) {
            return false;
        }

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
        if constexpr (constrainPaith) {
            if (pitch > 89.0f) { pitch = 89.0f; }
            if (pitch < -89.f) { pitch = -89.f; };
        }

        return true;
    }

    static float processMovementedSpeed(const FWindow* pWindow, float& movementSpeed,
                                        float deltaTime) {
        movementSpeed += pWindow->getScrollY();

        movementSpeed = [movementSpeed]()->float {
            if (movementSpeed < 1.0f) { return 1.0f; }
            if (movementSpeed > 90.f) { return 90.f; }
            return movementSpeed;
        }();

        return movementSpeed * deltaTime;
    }


    template<> bool FCameraMouseDecorator::update<FCamera3D>(FCamera3D* pCamera, int32 key) {
        const FWindow* pWindow{ pCamera->getWindow() };

        static bool firstMouse{ true };
        static bool holdingActivatedInput{ false };
        bool userPressedSth{ false };
        const bool userActivatedInput{ pWindow->isMousePressed(key) };

        if(userActivatedInput) {
            if(!holdingActivatedInput) {
                firstMouse = true;
            }

            const float velocity{ processMovementedSpeed(pWindow, pCamera->m_movementSpeed,
                                                         pCamera->getDeltaTime()) };
            if(velocity != pCamera->m_velocity) {
                pCamera->m_velocity = velocity;
                userPressedSth = true;
            }

            bool updatedAngles =
                    processQuatAngles(pWindow, pCamera->m_yaw, pCamera->m_pitch, firstMouse);
            if(updatedAngles) {
                firstMouse = false;
                userPressedSth = true;
            }
        }
        else {
            firstMouse = true;
        }

        holdingActivatedInput = userActivatedInput;

        return userPressedSth;
    }


}

