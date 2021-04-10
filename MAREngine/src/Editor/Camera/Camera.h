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


#ifndef MAR_ENGINE_EDITOR_CAMERA_H
#define MAR_ENGINE_EDITOR_CAMERA_H


#include "../../mar.h"
#include "StandardCamera.h"
#include "SphericalCamera.h"
#include "../../Core/graphics/RenderCamera.h"


namespace marengine {

    class IWindow;


    class Camera {

        friend class StandardCamera;
        friend class SphericalCamera;

    public:

        void initialize(float aspectRatio);
        bool update(IWindow* pWindow, float aspectRatio, bool useInput);

        MAR_NO_DISCARD const RenderCamera* getCameraData() const;

    private:

        bool processInput(IWindow* pWindow);
        void updateCameraVectors();


        RenderCamera m_renderCamera;

        maths::vec3 m_position{ -10.0f, 2.0f, 0.0f };
        maths::vec3 m_front{ 0.0f, 0.0f, -1.0f };
        maths::vec3 m_up;
        maths::vec3 m_right;

        float m_yaw{ 0.0f };
        float m_pitch{ 0.0f };

        float m_movementSpeed{ 5.0f };
        float m_zoom{ 55.0f };

        float m_aspectRatio{ 1.333f };

        float m_near{ 0.001f };
        float m_far{ 10000.f };

        StandardCamera m_standard;
        SphericalCamera m_spherical;

    };


}


#endif // !MAR_ENGINE_EDITOR_CAMERA_H
