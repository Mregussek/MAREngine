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


#ifndef MARENGINE_IWINDOW_H
#define MARENGINE_IWINDOW_H


#include "../mar.h"
#include "buttons_def.h"


namespace marengine {


    class IWindow {
    public:

        virtual bool open(uint32_t width, uint32_t height, const char* name) = 0;
        virtual void close() = 0;

        virtual void terminateLibrary() = 0;

        virtual void initEditorGuiLibrary() = 0;
        virtual void beginNewFrameEditorGuiLibrary() = 0;
        virtual void terminateEditorGuiLibrary() = 0;

        MAR_NO_DISCARD virtual bool isGoingToClose() = 0;

        virtual void setVerticalSync(int32_t vsSetValue) = 0;

        virtual void swapBuffers() = 0;

        MAR_NO_DISCARD virtual int32_t getSizeX() const = 0;
        MAR_NO_DISCARD virtual int32_t getSizeY() const = 0;
        MAR_NO_DISCARD virtual bool isKeyPressed(int32_t key) const = 0;
        MAR_NO_DISCARD virtual bool isMousePressed(int32_t key) const = 0;
        MAR_NO_DISCARD virtual float getMousePositionX() const = 0;
        MAR_NO_DISCARD virtual float getMousePositionY() const = 0;
        MAR_NO_DISCARD virtual float getScrollX() const = 0;
        MAR_NO_DISCARD virtual float getScrollY() const = 0;

    };


    struct FWindowCurrentInfo {

        uint32_t width{ 800 };
        uint32_t height{ 600 };
        float xScroll{ 0.f };
        float yScroll{ 0.f };
        float xMousePos{ 0.f };
        float yMousePos{ 0.f };

    };


}


#endif //MARENGINE_IWINDOW_H
