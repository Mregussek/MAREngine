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
#include "../public/Framebuffer.h"


namespace marengine {


    void FCamera::create(FWindow* pWindow, FFramebuffer* pFramebuffer) {
        p_pFramebuffer = pFramebuffer;
        p_pWindow = pWindow;
    }

    bool FCamera::update() {
        static float lastTime{ 0.0f };

        const float currentFrame{ (float)glfwGetTime() };
        p_deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        return false;
    }

    void FCamera::recalculate() {

    }

    const FRenderCamera* FCamera::getRenderCamera() const {
        return &p_renderCamera;
    }

    const FWindow* FCamera::getWindow() const {
        return p_pWindow;
    }

    float FCamera::getDeltaTime() const {
        return p_deltaTime;
    }


    void FCamera3D::create(FWindow* pWindow, FFramebuffer* pFramebuffer) {
        FCamera::create(pWindow, pFramebuffer);
        m_aspectRatio = (float)p_pFramebuffer->getWidth() / (float)p_pFramebuffer->getHeight();

        p_renderCamera.calculatePerspective(m_zoom, m_aspectRatio, m_near, m_far);
        p_renderCamera.calculateModel({ 0.f, 0.f, 0.f });

        recalculate();
    }

    bool FCamera3D::update() {
        FCamera::update();

        const float aspectRatio =
                (float)p_pFramebuffer->getWidth() / (float)p_pFramebuffer->getHeight();

        if (m_aspectRatio != aspectRatio) {
            m_aspectRatio = aspectRatio;
            p_renderCamera.calculatePerspective(m_zoom, m_aspectRatio, m_near, m_far);
            p_renderCamera.recalculateMVP();
            return true;
        }

        return false;
    }

    void FCamera3D::recalculate() {
        typedef maths::vec3 vec3;
        typedef maths::trig tri;

        const vec3 worldUp{ 0.f, 1.f, 0.f };
        const float yawRad{ tri::toRadians(m_yaw) };
        const float pitchRad{ tri::toRadians(m_pitch) };

        const vec3 front{
            tri::cosine(yawRad) * tri::cosine(pitchRad),
            tri::sine(pitchRad),
            tri::sine(yawRad) * tri::cosine(pitchRad)
        };

        m_front = vec3::normalize(front);
        m_right = vec3::normalize(vec3::cross(m_front, worldUp));
        m_up = vec3::normalize(vec3::cross(m_right, m_front));

        p_renderCamera.calculateView(m_position, m_position + m_front, m_up);
        p_renderCamera.recalculateMVP();
    }


}
