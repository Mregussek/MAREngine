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
#include "../../Window/Window.h"


namespace mar::editor {

    
    Camera* Camera::s_instance{ nullptr };
    
    // ---- PUBLIC METHODS ---- //
    
    void Camera::initialize(float aspectRatio) {
        s_instance = this;
        m_aspectRatio = aspectRatio;
        updateCameraVectors();
        updateData();
    }
    
    void Camera::update(float aspectRatio) {
        processInput();
        //processMouseMovement(m_gui->getMouseViewportPosX(), m_gui->getMouseViewportPosY(), false, firstMouse);
        //processMouseScroll(window::Input::getScrollY());
    
        m_aspectRatio = aspectRatio;
        updateData();
    }
    
    // ---- PRIVATE METHODS ---- //
    
    void Camera::processInput() {
        static float lastTime = 0.0f;
    
        const float currentFrame = (float)glfwGetTime();
        const float deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        const float velocity = m_movementSpeed * deltaTime;
    
        if (window::isKeyPressed(MAR_KEY_W)) { m_position += m_front * velocity; }
        if (window::isKeyPressed(MAR_KEY_S)) { m_position -= m_front * velocity; }
        if (window::isKeyPressed(MAR_KEY_A)) { m_position -= m_right * velocity; }
        if (window::isKeyPressed(MAR_KEY_D)) { m_position += m_right * velocity; }
    }
    
    void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch, bool firstMouse) {
        static float lastX;
        static float lastY;
    
        if (firstMouse) {
            lastX = xoffset;
            lastY = yoffset;
        }
    
        float x_position = xoffset - lastX;
        float y_position = lastY - yoffset;

        lastX = xoffset;
        lastY = yoffset;
    
        x_position *= m_mouseSensitivity;
        y_position *= m_mouseSensitivity;
    
        m_yaw += x_position;
        m_pitch += y_position;
    
        if (constrainPitch) {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }
    
        updateCameraVectors();
    }
    
    void Camera::processMouseScroll(float yoffset) {
        m_zoom -= yoffset;

        if (m_zoom < 1.0f) { m_zoom = 1.0f; }
        if (m_zoom > 45.0f) { m_zoom = 45.0f; }
    }
    
    void Camera::updateData() {

        m_renderCamera.calculatePerspective(m_zoom, m_aspectRatio, 0.01f, 100.0f);
        m_renderCamera.calculateView(m_position, m_position + m_front, m_up);
        m_renderCamera.calculateModel({ 0.f, 0.f, 0.f });
        m_renderCamera.recalculateMVP();
 
    }
    
    void Camera::updateCameraVectors() {
        typedef maths::Trig tri;
    
        const maths::vec3 front{
            {tri::cosine(tri::toRadians(m_yaw)) * tri::cosine(tri::toRadians(m_pitch))},
            {tri::sine(tri::toRadians(m_pitch))},
            {tri::sine(tri::toRadians(m_yaw)) * tri::cosine(tri::toRadians(m_pitch))}
        };

        m_front = maths::vec3::normalize(front);
    
        m_right = maths::vec3::normalize(maths::vec3::cross(m_front, m_worldUp));
        m_up = maths::vec3::normalize(maths::vec3::cross(m_right, m_front));
    }

    graphics::RenderCamera& Camera::getCameraData() { 
        return s_instance->m_renderCamera; 
    }


}
