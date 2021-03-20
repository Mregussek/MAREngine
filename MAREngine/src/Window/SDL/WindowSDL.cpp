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


#include "WindowSDL.h"
#include "../../Logging/Logger.h"


namespace marengine {


    bool FWindowSDL::open(uint32_t width, uint32_t height, const char* name) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        constexpr SDL_WindowFlags windowFlags = []()->SDL_WindowFlags{
            if constexpr (MARENGINE_USE_OPENGL_RENDERAPI) {
                return (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
            }
            else {
                MARLOG_CRIT(ELoggerType::NORMAL, "RenderAPI is not set, cannot initialize SDL window!");
                FLogger::callDebugBreak(true);
                return SDL_WindowFlags();
            }
        }();

        p_pWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
        p_context = SDL_GL_CreateContext(p_pWindow);
        const auto isContextOK = SDL_GL_MakeCurrent(p_pWindow, p_context);
        if (isContextOK == SDL_FALSE) {
            MARLOG_CRIT(ELoggerType::NORMAL, "cannot initialize SDL window, SDL context could not be created!");
            FLogger::callDebugBreak(true);
            return false;
        }

        setVerticalSync(1);

        return true;
    }

    void FWindowSDL::close() {
        p_shouldWindowClose = true;
    }

    bool FWindowSDL::initializeLibrary() {
        const auto isLibraryOK = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (isLibraryOK == SDL_FALSE) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize SDL library!");
            FLogger::callDebugBreak(true);
            return false;
        }

        return true;
    }

    void FWindowSDL::terminateLibrary() {
        SDL_GL_DeleteContext(p_context);
        SDL_DestroyWindow(p_pWindow);
        SDL_Quit();
    }

    bool FWindowSDL::isGoingToClose() {
        return p_shouldWindowClose;
    }

    void FWindowSDL::setVerticalSync(int32_t vsSetValue) {
        SDL_GL_SetSwapInterval(vsSetValue);
    }

    void FWindowSDL::swapBuffers() {
        // TODO: add current info for SDL window
        //p_currentInfo.width = callbacks::window_width;
        //p_currentInfo.height = callbacks::window_height;
        //p_currentInfo.xMousePos = (float)callbacks::mouse_xpos;
        //p_currentInfo.yMousePos = (float)callbacks::mouse_ypos;
        //p_currentInfo.xScroll = (float)callbacks::scroll_x;
        //p_currentInfo.yScroll = (float)callbacks::scroll_y;

        SDL_GL_SwapWindow(p_pWindow);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                p_shouldWindowClose = true;
            }
        }
    }

    int32_t FWindowSDL::getSizeX() const {
        return p_currentInfo.width;
    }

    int32_t FWindowSDL::getSizeY() const {
        return p_currentInfo.height;
    }

    bool FWindowSDL::isKeyPressed(int32_t key) const {
        return false;
    }

    bool FWindowSDL::isMousePressed(int32_t key) const {
        return false;
    }

    float FWindowSDL::getMousePositionX() const {
        return p_currentInfo.xMousePos;
    }

    float FWindowSDL::getMousePositionY() const {
        return p_currentInfo.yMousePos;
    }

    float FWindowSDL::getScrollX() const {
        return p_currentInfo.xScroll;
    }

    float FWindowSDL::getScrollY() const {
        return p_currentInfo.yScroll;
    }


    void FWindowSDLImGui::initEditorGuiLibrary() {
        if constexpr (MARENGINE_USE_OPENGL_RENDERAPI) {
            ImGui_ImplSDL2_InitForOpenGL(p_pWindow, p_context);
        }
        else {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot create ImGui in SDL2 window, because RenderAPI is not supported!");
            FLogger::callDebugBreak(true);
        }
    }

    void FWindowSDLImGui::beginNewFrameEditorGuiLibrary() {
        ImGui_ImplSDL2_NewFrame(p_pWindow);
    }

    void FWindowSDLImGui::terminateEditorGuiLibrary() {
        ImGui_ImplSDL2_Shutdown();
    }


}
