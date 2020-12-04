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


#include "Camera.h"
#include "StandardCamera.h"
#include "SphericalCamera.h"
#include "../../Window/Window.h"


namespace mar::editor {

    
    void Camera::initialize(float aspectRatio) {
        m_aspectRatio = aspectRatio;

        updateCameraVectors();
        updateData();
    }
    
    void Camera::update(float aspectRatio) {
        m_aspectRatio = aspectRatio;

        if (processInput()) {
            updateCameraVectors();
            updateData();
        }
    }
    
    // ---- PRIVATE METHODS ---- //
    
    bool Camera::processInput() {
        static float lastTime = 0.0f;
        bool userPressedSth = false;
    
        const float currentFrame = (float)glfwGetTime();
        const float deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        
        if (m_standard.processFrame(this, deltaTime)) { userPressedSth = true; }
        if (m_spherical.processFrame(this, deltaTime)) { userPressedSth = true; }

        return userPressedSth;
    }
    
    void Camera::updateData() {
        m_renderCamera.calculatePerspective(m_zoom, m_aspectRatio, 0.01f, 100.0f);
        m_renderCamera.calculateView(m_position, m_position + m_front, m_up);
        m_renderCamera.calculateModel({ 0.f, 0.f, 0.f });
        m_renderCamera.recalculateMVP();
    }
    
    void Camera::updateCameraVectors() {
        typedef maths::trig tri;

        const maths::vec3 worldUp{ 0.f, 1.f, 0.f };
    
        const maths::vec3 front{
            {tri::cosine(tri::toRadians(m_yaw)) * tri::cosine(tri::toRadians(m_pitch))},
            {tri::sine(tri::toRadians(m_pitch))},
            {tri::sine(tri::toRadians(m_yaw)) * tri::cosine(tri::toRadians(m_pitch))}
        };

        m_front = maths::vec3::normalize(front);
        m_right = maths::vec3::normalize(maths::vec3::cross(m_front, worldUp));
        m_up = maths::vec3::normalize(maths::vec3::cross(m_right, m_front));
    }

    const graphics::RenderCamera* Camera::getCameraData() const { 
        return &m_renderCamera; 
    }


}
