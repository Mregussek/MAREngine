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


#include "Camera.h"
#include "StandardCamera.h"
#include "SphericalCamera.h"
#include "Window/Window.h"


namespace marengine {

    
    void Camera::initialize(float aspectRatio) {
        m_aspectRatio = aspectRatio;

        updateCameraVectors();

        m_renderCamera.calculatePerspective(m_zoom, m_aspectRatio, m_near, m_far);
        m_renderCamera.calculateView(m_position, m_position + m_front, m_up);
        m_renderCamera.calculateModel({ 0.f, 0.f, 0.f });
        m_renderCamera.recalculateMVP();
    }
    
    bool Camera::update(IWindow* pWindow, float aspectRatio, bool useInput) {
        auto perspectiveUpdateWasNeeded = [this, aspectRatio]()->bool {
            if (m_aspectRatio != aspectRatio) {
                m_aspectRatio = aspectRatio;
                m_renderCamera.calculatePerspective(m_zoom, m_aspectRatio, m_near, m_far);
                return true;
            }

            return false;
        };

        if (useInput) {
            const bool userPressedSomeKey{ processInput(pWindow) };
            if (userPressedSomeKey) {
                updateCameraVectors();
                perspectiveUpdateWasNeeded();
                m_renderCamera.calculateView(m_position, m_position + m_front, m_up);
                m_renderCamera.recalculateMVP();
                return true; 
            }
            else {
                if (perspectiveUpdateWasNeeded()) {
                    m_renderCamera.recalculateMVP(); 
                    return true;
                }
            }
        }
        else {
            if (perspectiveUpdateWasNeeded()) {
                m_renderCamera.recalculateMVP(); 
                return true; 
            }
        }

        return false;
    }
    
    // ---- PRIVATE METHODS ---- //

    bool Camera::processInput(IWindow* pWindow) {
        static float lastTime{ 0.0f };
        bool userPressedSth{ false };
    
        const float currentFrame{ (float)glfwGetTime() };
        const float deltaTime{ currentFrame - lastTime };
        lastTime = currentFrame;
        
        if (m_standard.processFrame(this, pWindow, deltaTime)) { userPressedSth = true; }
        if (m_spherical.processFrame(this, pWindow, deltaTime)) { userPressedSth = true; }

        return userPressedSth;
    }

    void Camera::updateCameraVectors() {
        typedef maths::vec3 vec3;
        typedef maths::trig tri;

        const maths::vec3 worldUp{ 0.f, 1.f, 0.f };
        const auto yawRad{ tri::toRadians(m_yaw) };
        const auto pitchRad{ tri::toRadians(m_pitch) };

        const maths::vec3 front{
            { tri::cosine(yawRad) * tri::cosine(pitchRad) },
            { tri::sine(pitchRad) },
            { tri::sine(yawRad) * tri::cosine(pitchRad) }
        };

        m_front = vec3::normalize(front);
        m_right = vec3::normalize(vec3::cross(m_front, worldUp));
        m_up = vec3::normalize(vec3::cross(m_right, m_front));
    }

    const FRenderCamera* Camera::getCameraData() const {
        return &m_renderCamera; 
    }


}
