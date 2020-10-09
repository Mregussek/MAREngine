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
#include "../../Window/Input.h"


namespace mar::editor {


    static const float YAW = 0.0f;
    static const float PITCH = 0.0f;
    static const float SPEED = 5.0f;
    static const float SENSITIVITY = 0.1f;
    static const float ZOOM = 55.0f;
    
    Camera* Camera::s_instance{ nullptr };
    
    // ---- PUBLIC METHODS ---- //
    
    Camera::Camera()
        : m_front(maths::vec3(0.0f, 0.0f, -1.0f)),
        m_movementSpeed(SPEED),
        m_mouseSensitivity(SENSITIVITY),
        m_zoom(ZOOM),
        m_aspectRatio(1.333f),
        m_position(-10.0f, 2.0f, 0.0f),
        m_worldUp(0.0f, 1.0f, 0.0f),
        m_yaw(YAW),
        m_pitch(PITCH)
    {
        updateCameraVectors();
    }
    
    void Camera::initialize(float aspectRatio) {
        s_instance = this;
        aspectRatio = aspectRatio;
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
        static float currentFrame;
        static float deltaTime;
        static float lastTime = 0.0f;
        static float velocity;
    
        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;
        velocity = m_movementSpeed * deltaTime;
    
        if (window::Input::isKeyPressed(MAR_KEY_W)) { m_position += m_front * velocity; }
        if (window::Input::isKeyPressed(MAR_KEY_S)) { m_position -= m_front * velocity; }
        if (window::Input::isKeyPressed(MAR_KEY_A)) { m_position -= m_right * velocity; }
        if (window::Input::isKeyPressed(MAR_KEY_D)) { m_position += m_right * velocity; }
    }
    
    void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch, bool firstMouse) {
        static float lastX;
        static float lastY;
        static float x_position;
        static float y_position;
    
        if (firstMouse) {
            lastX = xoffset;
            lastY = yoffset;
        }
    
        x_position = xoffset - lastX;
        y_position = lastY - yoffset;
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
        m_zoom -= (float)yoffset;
        if (m_zoom < 1.0f) { m_zoom = 1.0f; }
        if (m_zoom > 45.0f) { m_zoom = 45.0f; }
    }
    
    void Camera::updateData() {
        typedef maths::mat4 m4;
        typedef maths::Trig tri;
    
        m_renderCamera.projection = m4::perspective(tri::toRadians(m_zoom), m_aspectRatio, 0.01f, 100.0f);
        m_renderCamera.view = m4::lookAt(m_position, m_position + m_front, m_up);
        m_renderCamera.model = m4::translation({ 0.0f, 0.0f, 0.0f });
    
        m_renderCamera.mvp = m_renderCamera.projection * m_renderCamera.view;
    
        m_renderCamera.position = m_position;
    }
    
    void Camera::updateCameraVectors() {
        typedef maths::Trig tri;
        static maths::vec3 front;
    
        front.x = tri::cosine(tri::toRadians(m_yaw)) * tri::cosine(tri::toRadians(m_pitch));
        front.y = tri::sine(tri::toRadians(m_pitch));
        front.z = tri::sine(tri::toRadians(m_yaw)) * tri::cosine(tri::toRadians(m_pitch));
        m_front = maths::vec3::normalize(front);
    
        m_right = maths::vec3::normalize(maths::vec3::cross(m_front, m_worldUp));
        m_up = maths::vec3::normalize(maths::vec3::cross(m_right, m_front));
    }


}
