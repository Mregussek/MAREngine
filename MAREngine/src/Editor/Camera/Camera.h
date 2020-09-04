/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_CAMERA_H
#define MAR_ENGINE_EDITOR_CAMERA_H

#include "../../mar.h"
#include "../../Window/Input.h"
#include "../../Core/graphics/Renderer/RenderCamera.h"


namespace mar {
	namespace editor {

		
        enum class Camera_Movement {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };

        const float YAW = 0.0f;
        const float PITCH = 0.0f;
        const float SPEED = 5.0f;
        const float SENSITIVITY = 0.1f;
        const float ZOOM = 55.0f;


        class Camera {
        public:

            maths::vec3 Position;
            maths::vec3 Front;
            maths::vec3 Up;
            maths::vec3 Right;
            maths::vec3 WorldUp;

            float Yaw;
            float Pitch;

            float MovementSpeed;
            float MouseSensitivity;
            float Zoom;

            float aspectRatio;

            static graphics::RenderCamera s_cameraData;

            Camera()
                : Front(maths::vec3(0.0f, 0.0f, -1.0f)), 
                MovementSpeed(SPEED), 
                MouseSensitivity(SENSITIVITY), 
                Zoom(ZOOM),
                aspectRatio(1.333f)
            {
                Position = maths::vec3(-10.0f, 2.0f, 0.0f);
                WorldUp = maths::vec3(0.0f, 1.0f, 0.0f);
                Yaw = YAW;
                Pitch = PITCH;
                updateCameraVectors();
            }

            void initialize() {
                updateCameraVectors();
            }

            inline static graphics::RenderCamera& getCameraData() { return s_cameraData; }

            maths::mat4 getProjectionMatrix() const {
                return maths::mat4::perspective(maths::Trig::toRadians(Zoom), aspectRatio, 0.01f, 100.0f);
            }

            maths::mat4 getViewMatrix() const {
                return maths::mat4::lookAt(Position, Position + Front, Up);
            }

            maths::mat4 getModelMatrix() const {
                return maths::mat4::translation({ 0.0f, 0.0f, 0.0f });
            }

            void processInput() {
                static float currentFrame;
                static float deltaTime;
                static float lastTime = 0.0f;
                static float velocity;

                currentFrame = (float)glfwGetTime();
                deltaTime = currentFrame - lastTime;
                lastTime = currentFrame;
                velocity = MovementSpeed * deltaTime;

                if (window::Input::isKeyPressed(MAR_KEY_W)) {
                    Position += Front * velocity;
                }
                if (window::Input::isKeyPressed(MAR_KEY_S)) {
                    Position -= Front * velocity;
                }
                if (window::Input::isKeyPressed(MAR_KEY_A)) {
                    Position -= Right * velocity;
                }
                if (window::Input::isKeyPressed(MAR_KEY_D)) {
                    Position += Right * velocity;
                }
            }

            void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch, bool firstMouse) {
                static float lastX;
                static float lastY;

                if (firstMouse) {
                    std::cout << "xoffset: " << xoffset << " yoffset: " << yoffset << "firstMouse: " << firstMouse << std::endl;
                    lastX = xoffset;
                    lastY = yoffset;
                }

                static float x_position;
                static float y_position;

                x_position = xoffset - lastX;
                y_position = lastY - yoffset;
                lastX = xoffset;
                lastY = yoffset;

                x_position *= MouseSensitivity;
                y_position *= MouseSensitivity;

                Yaw += x_position;
                Pitch += y_position;

                if (constrainPitch) {
                    if (Pitch > 89.0f)
                        Pitch = 89.0f;
                    if (Pitch < -89.0f)
                        Pitch = -89.0f;
                }

                updateCameraVectors();
            }

            void ProcessMouseScroll(float yoffset) {
                Zoom -= (float)yoffset;
                if (Zoom < 1.0f)
                    Zoom = 1.0f;
                if (Zoom > 45.0f)
                    Zoom = 45.0f;
            }

            void updateData() {
                s_cameraData.projection = getProjectionMatrix();
                s_cameraData.model = getModelMatrix();
                s_cameraData.view = getViewMatrix();

                s_cameraData.mvp = s_cameraData.projection * s_cameraData.view;

                s_cameraData.position = Position;
            }

        private:
            void updateCameraVectors(){
                static maths::vec3 front;

                front.x = maths::Trig::cosine(maths::Trig::toRadians(Yaw)) * maths::Trig::cosine(maths::Trig::toRadians(Pitch));
                front.y = maths::Trig::sine(maths::Trig::toRadians(Pitch));
                front.z = maths::Trig::sine(maths::Trig::toRadians(Yaw)) * maths::Trig::cosine(maths::Trig::toRadians(Pitch));
                Front = maths::vec3::normalize(front);

                Right = maths::vec3::normalize(maths::vec3::cross(Front, WorldUp));
                Up = maths::vec3::normalize(maths::vec3::cross(Right, Front));
            }
        };


} }


#endif // !MAR_ENGINE_EDITOR_CAMERA_H