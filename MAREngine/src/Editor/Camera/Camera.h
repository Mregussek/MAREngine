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


#ifndef MAR_ENGINE_EDITOR_CAMERA_H
#define MAR_ENGINE_EDITOR_CAMERA_H


#include "../../mar.h"
#include "../../Core/graphics/Renderer/RenderCamera.h"


namespace mar {
	namespace editor {


        class Camera {
        public:

            Camera();

            void initialize(float aspectRatio);
            void update(float aspectRatio);

            static graphics::RenderCamera& getCameraData() { return s_instance->m_renderCamera; }

        private:

            void processInput();
            void processMouseMovement(float xoffset, float yoffset, bool constrainPitch, bool firstMouse);
            void processMouseScroll(float yoffset);
            void updateCameraVectors();

            void updateData();

            maths::vec3 m_position;
            maths::vec3 m_front;
            maths::vec3 m_up;
            maths::vec3 m_right;
            maths::vec3 m_worldUp;

            float m_yaw;
            float m_pitch;

            float m_movementSpeed;
            float m_mouseSensitivity;
            float m_zoom;

            float m_aspectRatio;

            graphics::RenderCamera m_renderCamera;

            static Camera* s_instance;
        };


} }


#endif // !MAR_ENGINE_EDITOR_CAMERA_H