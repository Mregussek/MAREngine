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


#include "WindowGLFW.h"
#include "WindowCallbacks.h"
#include "../../Logging/Logger.h"


namespace marengine {


    bool FWindowGLFW::open(uint32_t width, uint32_t height, const char* name) {
        MARLOG_TRACE(ELoggerType::WINDOW, "Opening GLFW window...");
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        p_pWindowContext = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if (!p_pWindowContext) {
            terminateLibrary();

            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot create GLFW window!");
            FLogger::callDebugBreak(true);
            return false;
        }

        glfwMakeContextCurrent(p_pWindowContext);
        glfwSetInputMode(p_pWindowContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glfwSetWindowSizeCallback(p_pWindowContext, callbacks::windowSizeCallback);
        //glfwSetFramebufferSizeCallback(p_pWindowContext, callbacks::framebufferSizeCallback);
        glfwSetCursorPosCallback(p_pWindowContext, callbacks::cursorPositionCallback);
        glfwSetScrollCallback(p_pWindowContext, callbacks::scrollCallback);
        glfwSetMouseButtonCallback(p_pWindowContext, callbacks::mouseButtonCallback);

        setVerticalSync(1);

        MARLOG_INFO(ELoggerType::WINDOW, "GLFW Window has opened successfully!");
        return true;
	}

    void FWindowGLFW::close() {
        glfwSetWindowShouldClose(p_pWindowContext, true);
	}

	bool FWindowGLFW::initializeLibrary() {
        MARLOG_TRACE(ELoggerType::WINDOW, "Initializing GLFW library...");
        glfwSetErrorCallback(callbacks::windowErrorCallback);
        const int32_t glfw_init{ glfwInit() };

        if (glfw_init != GLFW_TRUE) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize GLFW library!");
            FLogger::callDebugBreak(true);
            return false;
        }

        MARLOG_INFO(ELoggerType::WINDOW, "Initialized GLFW!");
        return true;
    }

    void FWindowGLFW::terminateLibrary() {
        glfwTerminate();
	}

    bool FWindowGLFW::isGoingToClose() {
        return glfwWindowShouldClose(p_pWindowContext);
    }

    void FWindowGLFW::setVerticalSync(int32_t vsSetValue) {
        MARLOG_TRACE(ELoggerType::WINDOW, "Setting Vertical Sync...")
        glfwSwapInterval(vsSetValue);
	}

	void FWindowGLFW::clear() const {
        //GL_FUNC( glViewport(0, 0, p_currentInfo.width, p_currentInfo.height) );
        GL_FUNC( glClearColor(p_currentInfo.clearColor.x, p_currentInfo.clearColor.y,
                              p_currentInfo.clearColor.z, 1.0f) );
        GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
    }

    void FWindowGLFW::swapBuffers() {
        p_currentInfo.width = callbacks::window_width;
        p_currentInfo.height = callbacks::window_height;
        p_currentInfo.xMousePos = (float) callbacks::mouse_xpos;
        p_currentInfo.yMousePos = (float) callbacks::mouse_ypos;
        p_currentInfo.xScroll = (float) callbacks::scroll_x;
        p_currentInfo.yScroll = (float) callbacks::scroll_y;

        glfwPollEvents();
        glfwSwapBuffers(p_pWindowContext);
    }

    bool FWindowGLFW::isKeyPressed(int32_t key) const {
        return glfwGetKey(p_pWindowContext, key) == GLFW_PRESS || glfwGetKey(p_pWindowContext, key) == GLFW_REPEAT;
    }

    bool FWindowGLFW::isMousePressed(int32_t key) const {
        return glfwGetMouseButton(p_pWindowContext, key) == GLFW_PRESS || glfwGetMouseButton(p_pWindowContext, key) == GLFW_REPEAT;
    }


    void FWindowGLFWImGui::initEditorGuiLibrary() {
        if constexpr (MARENGINE_USE_OPENGL_RENDERAPI) {
            ImGui_ImplGlfw_InitForOpenGL(p_pWindowContext, true);
        }
        else {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot create ImGui in GLFW window, because RenderAPI is not supported!");
            FLogger::callDebugBreak(true);
        }
    }

    void FWindowGLFWImGui::beginNewFrameEditorGuiLibrary() {
        ImGui_ImplGlfw_NewFrame();
    }

    void FWindowGLFWImGui::terminateEditorGuiLibrary() {
        ImGui_ImplGlfw_Shutdown();
    }


}
