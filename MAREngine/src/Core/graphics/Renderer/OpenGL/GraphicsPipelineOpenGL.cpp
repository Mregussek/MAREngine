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


#include "GraphicsPipelineOpenGL.h"
#include "../Buffer.h"
#include "../ShaderPipeline.h"
#include "../../../../Logging/Logger.h"


namespace marengine {

    static void createVAO(uint32_t& vao, const FVertexInputDescription& inputDescription);

    static void closeBuffer(FBuffer* pBuffer) {
        pBuffer->free();
        pBuffer->destroy();
    }



    void FGraphicsPipelineColorMeshOpenGL::create() {
        createVAO(m_vao, p_pVertexBuffer->getInputDescription());
    }

    void FGraphicsPipelineColorMeshOpenGL::close() {
        GL_FUNC( glDeleteVertexArrays(1, &m_vao) );
        closeBuffer(p_pVertexBuffer);
        closeBuffer(p_pIndexBuffer);
        closeBuffer(p_pTransformsBuffer);
        closeBuffer(p_pColorsBuffer);
        p_pShaderPipeline->close();
    }

    void FGraphicsPipelineColorMeshOpenGL::bind() {
        GL_FUNC( glBindVertexArray(m_vao) );
        p_pVertexBuffer->bind();
        p_pIndexBuffer->bind();
        p_pTransformsBuffer->bind();
        p_pColorsBuffer->bind();
        p_pCameraBuffer->bind();
        p_pShaderPipeline->bind();
    }


    void FGraphicsPipelineTexture2DMeshOpenGL::create() {

    }

    void FGraphicsPipelineTexture2DMeshOpenGL::close() {

    }

    void FGraphicsPipelineTexture2DMeshOpenGL::bind() {

    }



    static GLenum getType(EBufferInputType inputType) {
        switch(inputType) {
        case EBufferInputType::FLOAT:
        case EBufferInputType::VEC4:
        case EBufferInputType::VEC3:
        case EBufferInputType::VEC2: 
            return GL_FLOAT;
        default:
            MARLOG_CRIT(ELoggerType::PLATFORMS, "Passed wrong EBufferInputType during VAO creation - {}", inputType);
            FLogger::callDebugBreak(true);
            return 0;
        }
    }

    static uint32_t getCount(EBufferInputType inputType) {
        switch(inputType) {
        case EBufferInputType::FLOAT: return 1;
        case EBufferInputType::VEC4: return 4;
        case EBufferInputType::VEC3: return 3;
        case EBufferInputType::VEC2: return 2;
        default:
            MARLOG_CRIT(ELoggerType::PLATFORMS, "Passed wrong EBufferInputType during VAO creation - {}", inputType);
            FLogger::callDebugBreak(true);
            return 0;
        }
    }
    
    void createVAO(uint32_t& vao, const FVertexInputDescription& inputDescription) {
        GL_FUNC( glGenVertexArrays(1, &vao) );
		GL_FUNC( glBindVertexArray(vao) );

        const uint32_t layoutsSize{ inputDescription.layoutArray.size() };
        for(uint32_t i = 0; i < layoutsSize; i++) {
            GL_FUNC( glVertexAttribPointer(i,
                                           getCount(inputDescription.layoutArray.at(i).inputType),
                                           getType(inputDescription.layoutArray.at(i).inputType),
                                           GL_FALSE,
                                           inputDescription.stride,
                                           (const void*)inputDescription.layoutArray.at(i).offset) );
		    GL_FUNC( glEnableVertexAttribArray(i) );
        }
    }


}
