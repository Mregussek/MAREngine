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


#include "GraphicsOpenGL.h"
#include "../../../../Logging/Logger.h"
#include "../../../../Platform/ShellTerminal/TerminalAPI.h"


namespace marengine {


    static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                           GLenum severity, GLsizei length,
                                           const GLchar* message, const void* userParam);


    bool FRenderContextOpenGL::create(FWindow* pWindow) {
        MARLOG_TRACE(ELoggerType::PLATFORMS, "Creating OpenGL Context...");
        m_pWindow = pWindow;

        if constexpr (MARENGINE_USE_GLFW_WINDOW) {
            MARLOG_TRACE(ELoggerType::PLATFORMS, "Loading GLFW into OpenGL...");
            const int32_t isGLAD_OK = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            if (!isGLAD_OK) {
                return false;
            }
        }
        else if constexpr (MARENGINE_USE_SDL_WINDOW) {
            MARLOG_TRACE(ELoggerType::PLATFORMS, "Loading SDL into OpenGL...");
            const int32_t isGLAD_OK = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
            if (!isGLAD_OK) {
                return false;
            }
        }
        else {
            return false;
        }

        if constexpr (MARENGINE_LOGGING_LEVEL != 0) {
            PLATFORM_GL_FUNC( glEnable(GL_DEBUG_OUTPUT) );
            PLATFORM_GL_FUNC( glDebugMessageCallback(MessageCallback, 0) );
        }

        PLATFORM_GL_FUNC( glEnable(GL_MULTISAMPLE) );
        PLATFORM_GL_FUNC( glEnable(GL_DEPTH_TEST) );
        PLATFORM_GL_FUNC( glEnable(GL_STENCIL_TEST) );
        PLATFORM_GL_FUNC( glEnable(GL_BLEND) );
        PLATFORM_GL_FUNC( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

        const GLubyte* vendor{ glGetString(GL_VENDOR) };
        const GLubyte* renderer{ glGetString(GL_RENDERER) };
        TerminalAPI::clearScreen();
        MARLOG_INFO(ELoggerType::PLATFORMS, "\nVendor: {}\nRenderer: {}", vendor, renderer);

        m_framebufferFactory.passRenderContext(this);
        m_pipelineFactory.passRenderContext(this);
        m_materialFactory.passRenderContext(this);
        m_shadersFactory.passRenderContext(this);
        m_bufferFactory.passRenderContext(this);

        return true;
    }

    void FRenderContextOpenGL::close() {

    }

    void FRenderContextOpenGL::prepareFrame() {
        const maths::vec3 backgroundColor{ 0.5f, 0.5f, 0.5f };
        PLATFORM_GL_FUNC( glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f) );
        PLATFORM_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
    }

    void FRenderContextOpenGL::endFrame() {

    }

    ERenderContextType FRenderContextOpenGL::getType() const {
        return ERenderContextType::OPENGL;
    }

    FBufferStorage* FRenderContextOpenGL::getBufferStorage() const {
        return (FBufferStorage*)&m_bufferFactory.m_storage;
    }

    FShadersStorage* FRenderContextOpenGL::getShadersStorage() const {
        return (FShadersStorage*)&m_shadersFactory.m_storage;
    }

    FPipelineStorage* FRenderContextOpenGL::getPipelineStorage() const {
        return (FPipelineStorage*)&m_pipelineFactory.m_storage;
    }

    FFramebufferStorage* FRenderContextOpenGL::getFramebufferStorage() const {
        return (FFramebufferStorage*)&m_framebufferFactory.m_storage;
    }

    FMaterialStorage* FRenderContextOpenGL::getMaterialStorage() const {
        return (FMaterialStorage*)&m_materialFactory.m_storage;
    }

    FBufferFactory* FRenderContextOpenGL::getBufferFactory() const {
        return (FBufferFactory*)&m_bufferFactory;
    }

    FShadersFactory* FRenderContextOpenGL::getShadersFactory() const {
        return (FShadersFactory*)&m_shadersFactory;
    }

    FPipelineFactory* FRenderContextOpenGL::getPipelineFactory() const {
        return (FPipelineFactory*)&m_pipelineFactory;
    }

    FFramebufferFactory* FRenderContextOpenGL::getFramebufferFactory() const {
        return (FFramebufferFactory*)&m_framebufferFactory;
    }

    FMaterialFactory* FRenderContextOpenGL::getMaterialFactory() const {
        return (FMaterialFactory*)&m_materialFactory;
    }


    static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                           GLenum severity, GLsizei length,
                                           const GLchar* message, const void* userParam)
    {
        constexpr bool displayDebugTypeOther{ false };
        if constexpr (!displayDebugTypeOther) {
            if (type == GL_DEBUG_TYPE_OTHER) {
                return;
            }
        }

        const char* errorType = [type]()->const char* {
            switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "PERFORMANCE";
            case GL_DEBUG_TYPE_OTHER:
                return "OTHER";
            default:
                return "UNKNOWN";
            }
        }();

        const char* severityType = [severity]()->const char* {
            switch (severity) {
            case GL_DEBUG_SEVERITY_LOW:
                return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH:
                return "HIGH";
            default:
                return "OTHER";
            }
        }();

        MARLOG_CRIT(ELoggerType::PLATFORMS, "OPENGL type={}, id={}, severity={}, message:\n{}",
                                            errorType, id, severityType, message);
     }


}
