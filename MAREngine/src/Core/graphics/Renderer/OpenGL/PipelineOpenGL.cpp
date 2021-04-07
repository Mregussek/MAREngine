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


#include "PipelineOpenGL.h"
#include "../Buffer.h"
#include "../Shaders.h"
#include "../../../../Logging/Logger.h"


namespace marengine {

    static void createVAO(uint32_t& vao, const FVertexInputDescription& inputDescription);


    void FPipelineMeshColorOpenGL::create() {
        createVAO(m_vao, p_pBufferStorage->getVBO(p_vboIndex)->getInputDescription());
    }

    void FPipelineMeshColorOpenGL::close() {
        GL_FUNC( glDeleteVertexArrays(1, &m_vao) );
    }

    void FPipelineMeshColorOpenGL::bind() {
        GL_FUNC( glBindVertexArray(m_vao) );
        p_pBufferStorage->getVBO(p_vboIndex)->bind();
        p_pBufferStorage->getIBO(p_iboIndex)->bind();
        p_pBufferStorage->getSSBO(p_transformIndex)->bind();
        p_pBufferStorage->getSSBO(p_colorIndex)->bind();
        p_pBufferStorage->getSSBO(p_camIndex)->bind();
        p_pBufferStorage->getSSBO(p_pointLightIndex)->bind();
        p_pShadersStorage->get(p_shaderIndex)->bind();
    }


    void FPipelineMeshTex2DOpenGL::create() {

    }

    void FPipelineMeshTex2DOpenGL::close() {

    }

    void FPipelineMeshTex2DOpenGL::bind() {

    }


    FPipelineMeshColor* FPipelineStorageOpenGL::getColorMesh(size_t index) const {
        return (FPipelineMeshColor*)&m_colors.at(index);
    }

    FPipelineMeshTex2D* FPipelineStorageOpenGL::getTex2DMesh(size_t index) const {
        return (FPipelineMeshTex2D*)&m_texs2D.at(index);
    }

    size_t FPipelineStorageOpenGL::getCountColorMesh() const {
        return m_colors.size();
    }

    size_t FPipelineStorageOpenGL::getCountTex2DMesh() const {
        return m_texs2D.size();
    }

    void FPipelineStorageOpenGL::reset() {
        for(auto& pipeline : m_colors) {
            pipeline.close();
        }
        for(auto& pipeline : m_texs2D) {
            pipeline.close();
        }
    }


    FPipelineMeshColor* FPipelineFactoryOpenGL::emplaceColorMesh() {
        return &m_storage.m_colors.emplace_back();
    }
    FPipelineMeshTex2D* FPipelineFactoryOpenGL::emplaceTex2DMesh() {
        return &m_storage.m_texs2D.emplace_back();
    }



    static GLenum getType(EInputType inputType) {
        switch(inputType) {
        case EInputType::FLOAT:
        case EInputType::VEC4:
        case EInputType::VEC3:
        case EInputType::VEC2:
            return GL_FLOAT;
        default:
            MARLOG_CRIT(ELoggerType::PLATFORMS, "Passed wrong EBufferInputType during VAO creation - {}", inputType);
            FLogger::callDebugBreak(true);
            return 0;
        }
    }

    static uint32_t getCount(EInputType inputType) {
        switch(inputType) {
        case EInputType::FLOAT: return 1;
        case EInputType::VEC4: return 4;
        case EInputType::VEC3: return 3;
        case EInputType::VEC2: return 2;
        default:
            MARLOG_CRIT(ELoggerType::PLATFORMS, "Passed wrong EBufferInputType during VAO creation - {}", inputType);
            FLogger::callDebugBreak(true);
            return 0;
        }
    }
    
    void createVAO(uint32_t& vao, const FVertexInputDescription& inputDescription) {
        GL_FUNC( glGenVertexArrays(1, &vao) );
		GL_FUNC( glBindVertexArray(vao) );

        const uint32_t layoutsSize{ inputDescription.inputVariables.size() };
        for(uint32_t i = 0; i < layoutsSize; i++) {
            const auto& inputVariable{ inputDescription.inputVariables.at(i) };

            GL_FUNC( glVertexAttribPointer(i,
                                           getCount(inputVariable.inputType),
                                           getType(inputVariable.inputType),
                                           GL_FALSE,
                                           inputDescription.stride,
                                           (const void*)inputVariable.offset) );
		    GL_FUNC( glEnableVertexAttribArray(i) );
        }
    }


}
