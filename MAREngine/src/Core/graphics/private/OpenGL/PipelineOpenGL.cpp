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
#include "../../public/Buffer.h"
#include "../../public/Shaders.h"
#include "../../public/Material.h"
#include "../../../../Logging/Logger.h"


namespace marengine {

    static void createVAO(uint32& vao, const FVertexInputDescription& inputDescription);


    void FPipelineMeshColorOpenGL::create() {
        createVAO(m_vao, p_pBufferStorage->getVBO(p_vboIndex)->getInputDescription());
    }

    void FPipelineMeshColorOpenGL::close() {
        GL_FUNC( glDeleteVertexArrays(1, &m_vao) );
    }

    void FPipelineMeshColorOpenGL::bind() const {
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
        createVAO(m_vao, p_pBufferStorage->getVBO(p_vboIndex)->getInputDescription());
    }

    void FPipelineMeshTex2DOpenGL::close() {
        GL_FUNC( glDeleteVertexArrays(1, &m_vao) );
    }

    template<typename TSamplerArray>
    static int32 getUniformLocation(const TSamplerArray& samplerArray, const char* name) {
        if (samplerArray.find(name) != samplerArray.end()) {
            return samplerArray.at(name);
        }

        MARLOG_ERR(ELoggerType::PLATFORMS, "Could not retrieve correct sampler location -> {}", name);
        return -1;
    }

    template<typename TSamplerArray>
    static void setUniformSamplerGL(const TSamplerArray& samplerArray, const char* name, int32 sampler) {
        GL_FUNC( glUniform1i(getUniformLocation(samplerArray, name), sampler) );
    }

    void FPipelineMeshTex2DOpenGL::bind() const {
        GL_FUNC( glBindVertexArray(m_vao) );
        p_pBufferStorage->getVBO(p_vboIndex)->bind();
        p_pBufferStorage->getIBO(p_iboIndex)->bind();
        p_pBufferStorage->getSSBO(p_transformIndex)->bind();
        p_pBufferStorage->getSSBO(p_camIndex)->bind();
        p_pBufferStorage->getSSBO(p_pointLightIndex)->bind();
        p_pShadersStorage->get(p_shaderIndex)->bind();
        const uint32 countMaterial{ p_pMaterialStorage->getCountTex2D() };
        for(uint32 i = 0; i < countMaterial; i++) {
            FMaterialTex2D* pTexture{ p_pMaterialStorage->getTex2D(m_textures.at(i)) };
            pTexture->bind();
            setUniformSamplerGL(m_samplerLocations, m_samplerNames[i], pTexture->getInfo().sampler);
        }
    }

    int32 FPipelineMeshTex2DOpenGL::discoverSamplerLocation(const char* samplerName) const {
        return glGetUniformLocation(p_pShadersStorage->get(p_shaderIndex)->getID(), samplerName);
    }


    FPipelineMeshColor* FPipelineStorageOpenGL::getColorMesh(int32 index) const {
        return (FPipelineMeshColor*)&m_colors.at(index);
    }

    FPipelineMeshTex2D* FPipelineStorageOpenGL::getTex2DMesh(int32 index) const {
        return (FPipelineMeshTex2D*)&m_texs2D.at(index);
    }

    uint32 FPipelineStorageOpenGL::getCountColorMesh() const {
        return m_colors.size();
    }

    uint32 FPipelineStorageOpenGL::getCountTex2DMesh() const {
        return m_texs2D.size();
    }

    void FPipelineStorageOpenGL::reset() {
        for(auto& pipeline : m_colors) {
            pipeline.close();
        }
        for(auto& pipeline : m_texs2D) {
            pipeline.close();
        }
        m_colors.clear();
        m_texs2D.clear();
    }


    template<typename TReturnType, typename TPipelineArray>
    static TReturnType* emplacePipelineAtArray(TPipelineArray& array) {
        auto& pip{ array.emplace_back() };
        const int8_t currentSize{ (int8_t)array.size() };
        pip.setIndex( currentSize - 1);
        return (TReturnType*)&pip;
    }

    FPipelineMeshColor* FPipelineFactoryOpenGL::emplaceMeshColor() {
        return emplacePipelineAtArray<FPipelineMeshColor>(m_storage.m_colors);
    }
    FPipelineMeshTex2D* FPipelineFactoryOpenGL::emplaceMeshTex2D() {
        return emplacePipelineAtArray<FPipelineMeshTex2D>(m_storage.m_texs2D);
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

    static uint32 getCount(EInputType inputType) {
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
    
    void createVAO(uint32& vao, const FVertexInputDescription& inputDescription) {
        GL_FUNC( glGenVertexArrays(1, &vao) );
		GL_FUNC( glBindVertexArray(vao) );

        const uint32 layoutsSize{ inputDescription.inputVariables.size() };
        for(uint32 i = 0; i < layoutsSize; i++) {
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
