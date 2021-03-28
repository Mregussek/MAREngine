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


#ifndef MARENGINE_WINDOWSDL_H
#define MARENGINE_WINDOWSDL_H


#include "../IWindow.h"


namespace marengine {


    class FWindowSDL : public FWindow {
    public:

        bool open(uint32_t width, uint32_t height, const char* name) final;
        void close() final;

        bool initializeLibrary() final;
        void terminateLibrary() final;

        MAR_NO_DISCARD bool isGoingToClose() final;

        void setVerticalSync(int32_t vsSetValue) final;
        void swapBuffers() final;

        MAR_NO_DISCARD bool isKeyPressed(int32_t key) const final;
        MAR_NO_DISCARD bool isMousePressed(int32_t key) const final;

    protected:

        SDL_Window* p_pWindow{ nullptr };
        SDL_GLContext p_context{};
        bool p_shouldWindowClose{ false };
    };


    class FWindowSDLImGui : public FWindowSDL {
    public:

        void initEditorGuiLibrary() final;
        void beginNewFrameEditorGuiLibrary() final;
        void terminateEditorGuiLibrary() final;

    };


}



#endif //MARENGINE_WINDOWSDL_H
