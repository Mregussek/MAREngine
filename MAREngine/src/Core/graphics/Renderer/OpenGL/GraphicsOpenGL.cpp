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


    bool FGraphicsContextOpenGL::create(FWindow* pWindow) {
        m_pWindow = pWindow;

        if constexpr (MARENGINE_USE_GLFW_WINDOW) {
            const int32_t isGLAD_OK = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            if (!isGLAD_OK) {
                return false;
            }
        }
        else if constexpr (MARENGINE_USE_SDL_WINDOW) {
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

        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        TerminalAPI::clearScreen();
        MARLOG_INFO(ELoggerType::PLATFORMS, "\nVendor: {}\nRenderer: {}", vendor, renderer);

        return true;
    }

    void FGraphicsContextOpenGL::close() {

    }

    void FGraphicsContextOpenGL::prepareFrame() {
        const maths::vec3 backgroundColor{ 0.5f, 0.5f, 0.5f };
        PLATFORM_GL_FUNC( glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f) );
        PLATFORM_GL_FUNC( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) );
    }

    void FGraphicsContextOpenGL::endFrame() {

    }

    EGraphicsContextType FGraphicsContextOpenGL::getType() const {
        return EGraphicsContextType::OPENGL;
    }

    FGraphicsFactory* FGraphicsContextOpenGL::getFactory() {
        return (FGraphicsFactory*)&m_factory;
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


    FShaderBuffer* const FGraphicsFactoryOpenGL::emplaceSSBO() {
        return &m_ssbos.emplace_back();
    }
    FShaderBuffer* const FGraphicsFactoryOpenGL::emplaceUBO() {
        return &m_ubos.emplace_back();
    }
    FVertexBuffer* const FGraphicsFactoryOpenGL::emplaceVBO() {
        return &m_vbos.emplace_back();
    }
    FIndexBuffer* const FGraphicsFactoryOpenGL::emplaceIBO() {
        return &m_ibos.emplace_back();
    }
    FShaderPipeline* const FGraphicsFactoryOpenGL::emplaceShaderPipeline() {
        return &m_shaderPipelines.emplace_back();
    }
    FGraphicsPipelineColorMesh* const FGraphicsFactoryOpenGL::emplacePipelineColorMesh() {
        return &m_pipelinesColorMesh.emplace_back();
    }
    FGraphicsPipelineTexture2DMesh* const FGraphicsFactoryOpenGL::emplacePipelineTexture2DMesh() {
        return &m_pipelinesTexture2DMesh.emplace_back();
    }

    size_t FGraphicsFactoryOpenGL::getCountSSBO() const {
        return m_ssbos.size();
    }
    size_t FGraphicsFactoryOpenGL::getCountUBO() const {
        return m_ubos.size();
    }
    size_t FGraphicsFactoryOpenGL::getCountVBO() const {
        return m_vbos.size();
    }
    size_t FGraphicsFactoryOpenGL::getCountIBO() const {
        return m_ibos.size();
    }
    size_t FGraphicsFactoryOpenGL::getCountShaderPipeline() const {
        return m_shaderPipelines.size();
    }
    size_t FGraphicsFactoryOpenGL::getCountPipelineColorMesh() const {
        return m_pipelinesColorMesh.size();
    }
    size_t FGraphicsFactoryOpenGL::getCountPipelineTexture2DMesh() const {
        return m_pipelinesTexture2DMesh.size();
    }

    FShaderBuffer* const FGraphicsFactoryOpenGL::getSSBO(size_t index) const {
        return (FShaderBuffer* const)&m_ssbos.at(index);
    }
    FShaderBuffer* const FGraphicsFactoryOpenGL::getUBO(size_t index) const {
        return (FShaderBuffer* const)&m_ubos.at(index);
    }
    FVertexBuffer* const FGraphicsFactoryOpenGL::getVBO(size_t index) const {
        return (FVertexBuffer* const)&m_vbos.at(index);
    }
    FIndexBuffer* const FGraphicsFactoryOpenGL::getIBO(size_t index) const {
        return (FIndexBuffer* const)&m_ibos.at(index);
    }
    FShaderPipeline* const FGraphicsFactoryOpenGL::getShaderPipeline(size_t index) const {
        return (FShaderPipeline* const)&m_shaderPipelines.at(index);
    }
    FGraphicsPipelineColorMesh* const FGraphicsFactoryOpenGL::getPipelineColorMesh(size_t index) const {
        return (FGraphicsPipelineColorMesh* const)&m_pipelinesColorMesh.at(index);
    }
    FGraphicsPipelineTexture2DMesh* const FGraphicsFactoryOpenGL::getPipelineTexture2DMesh(size_t index) const {
        return (FGraphicsPipelineTexture2DMesh* const)&m_pipelinesTexture2DMesh.at(index);
    }
    FRenderer2* const FGraphicsFactoryOpenGL::getRenderer() const {
        return (FRenderer2* const)&m_renderer;
    }

    FGraphicsPipelineMesh* const FGraphicsFactoryOpenGL::retrieveCorrectPipeline(EGraphicsPipelineType type,
                                                                                 size_t index) const {
        if (type == EGraphicsPipelineType::MESH_COLOR) {
            return getPipelineColorMesh(index);
        }
        if (type == EGraphicsPipelineType::MESH_TEXTURE2D) {
            return getPipelineTexture2DMesh(index);
        }

        return nullptr;
    }

    void FGraphicsFactoryOpenGL::reset() {
        auto clearBuffer = [](FBuffer* pBuffer){
            pBuffer->free();
            pBuffer->destroy();
        };
        for(auto& buffer : m_ssbos) { clearBuffer(&buffer); }
        for(auto& buffer : m_ubos) { clearBuffer(&buffer); }
        for(auto& buffer : m_vbos) { clearBuffer(&buffer); }
        for(auto& buffer : m_ibos) { clearBuffer(&buffer); }
        for(auto& type : m_shaderPipelines) { type.close(); }
        for(auto& type : m_pipelinesColorMesh) { type.close(); }
        for(auto& type : m_pipelinesTexture2DMesh) { type.close(); }

        m_ssbos.clear();
        m_ubos.clear();
        m_vbos.clear();
        m_ibos.clear();
        m_shaderPipelines.clear();
        m_pipelinesColorMesh.clear();
        m_pipelinesTexture2DMesh.clear();
    }


}
