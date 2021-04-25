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


namespace  marengine {


    template<>
    bool FCameraKeyboardDecorator::update<FCamera3D>(FCamera3D* pCamera, int32 key) {
        const FWindow* pWindow{ pCamera->getWindow() };
        if(!pWindow->isMousePressed(key)) {
            return false;
        }

        bool userPressedSth{ false };

        if (pWindow->isKeyPressed(MAR_KEY_W)) {
            pCamera->m_position =
                    pCamera->m_position + (pCamera->m_front * pCamera->m_velocity);
            userPressedSth = true;
        }
        if (pWindow->isKeyPressed(MAR_KEY_S)) {
            pCamera->m_position =
                    pCamera->m_position - (pCamera->m_front * pCamera->m_velocity);
            userPressedSth = true;
        }
        if (pWindow->isKeyPressed(MAR_KEY_A)) {
            pCamera->m_position =
                    pCamera->m_position - (pCamera->m_right * pCamera->m_velocity);
            userPressedSth = true;
        }
        if (pWindow->isKeyPressed(MAR_KEY_D)) {
            pCamera->m_position =
                    pCamera->m_position + (pCamera->m_right * pCamera->m_velocity);
            userPressedSth = true;
        }

        return userPressedSth;
    }


}
