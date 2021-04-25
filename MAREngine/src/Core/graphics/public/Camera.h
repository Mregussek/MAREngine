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


#ifndef MARENGINE_CAMERA_H
#define MARENGINE_CAMERA_H


#include "../../../mar.h"
#include "RenderCamera.h"


namespace marengine {

    class FWindow;
    class FFramebuffer;
    class FRenderCamera;


    class FCamera {
    public:

        virtual void create(FWindow* pWindow, FFramebuffer* pFramebuffer);
        virtual bool update();
        virtual void recalculate();

        MAR_NO_DISCARD virtual const FRenderCamera* getRenderCamera() const final;
        MAR_NO_DISCARD virtual const FWindow* getWindow() const final;
        MAR_NO_DISCARD virtual float getDeltaTime() const final;

    protected:

        FRenderCamera p_renderCamera;
        FWindow* p_pWindow{ nullptr };
        FFramebuffer* p_pFramebuffer{ nullptr };
        float p_deltaTime;

    };


    class FCamera3D : public FCamera {

        friend class FCameraKeyboardDecorator;
        friend class FCameraMouseDecorator;
        friend class FCameraSphericalDecorator;

    public:

        virtual void create(FWindow* pWindow, FFramebuffer* pFramebuffer) final;
        MAR_NO_DISCARD bool update() final;
        void recalculate() final;

    private:

        maths::vec3 m_position{ -10.0f, 2.0f, 0.0f };
        maths::vec3 m_front{ 0.0f, 0.0f, -1.0f };
        maths::vec3 m_up;
        maths::vec3 m_right;

        float m_yaw{ 0.0f };
        float m_pitch{ 0.0f };

        float m_movementSpeed{ 5.0f };
        float m_velocity{ 0.f };
        float m_zoom{ 55.0f };

        float m_aspectRatio{ 1.333f };

        float m_near{ 0.001f };
        float m_far{ 10000.f };

    };


    class FCameraKeyboardDecorator {
    public:

        template<typename TCamera>
        MAR_NO_DISCARD static bool update(TCamera* pCamera, int32 key);

    };


    class FCameraMouseDecorator {
    public:

        template<typename TCamera>
        MAR_NO_DISCARD static bool update(TCamera* pCamera, int32 key);

    };


    class FCameraSphericalDecorator {
    public:

        template<typename TCamera>
        MAR_NO_DISCARD static bool update(TCamera* pCamera, int32 key);

    };


}


#include "../private/Camera.inl"


#endif //MARENGINE_CAMERA_H
