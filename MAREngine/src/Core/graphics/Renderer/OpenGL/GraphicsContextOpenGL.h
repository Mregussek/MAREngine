/***********************************************************************
* @internal @copyright
*
*       MAREngine - open source 3D game engine
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


#ifndef MARENGINE_GRAPHICSCONTEXTOPENGL_H
#define MARENGINE_GRAPHICSCONTEXTOPENGL_H


#include "../../../../mar.h"
#include "../IGraphicsContext.h"
#include "RendererOpenGL.h"
#include "BufferOpenGL.h"
#include "GraphicsPipelineOpenGL.h"
#include "ShaderPipelineOpenGL.h"


namespace marengine {

    class FWindow;

    
    class FGraphicsFactoryOpenGL : public FGraphicsFactory {
    public:

        FShaderBuffer* emplaceSSBO() final {
            return &m_ssbos.emplace_back();
        }
        FShaderBuffer* emplaceUBO() final {
            return &m_ubos.emplace_back();
        }
        FVertexBuffer* emplaceVBO() final {
            return &m_vbos.emplace_back();
        }
        FIndexBuffer* emplaceIBO() final {
            return &m_ibos.emplace_back();
        }
        FShaderPipeline* emplaceShaderPipeline() final {
            return &m_shaderPipelines.emplace_back();
        }
        FGraphicsPipelineColorMesh* emplacePipelineColorMesh() final {
            return &m_pipelinesColorMesh.emplace_back();
        }
        FGraphicsPipelineTexture2DMesh* emplacePipelineTexture2DMesh() final {
            return &m_pipelinesTexture2DMesh.emplace_back();
        }

        size_t getCountSSBO() const final {
            return m_ssbos.size();
        }
        size_t getCountUBO() const final {
            return m_ubos.size();
        }
        size_t getCountVBO() const final {
            return m_vbos.size();
        }
        size_t getCountIBO() const final {
            return m_ibos.size();
        }
        size_t getCountShaderPipeline() const final {
            return m_shaderPipelines.size();
        }
        size_t getCountPipelineColorMesh() const final {
            return m_pipelinesColorMesh.size();
        }
        size_t getCountPipelineTexture2DMesh() const final {
            return m_pipelinesTexture2DMesh.size();
        }   

        FShaderBuffer* getSSBO(size_t index) final {
            return &m_ssbos.at(index);
        }
        FShaderBuffer* getUBO(size_t index) final {
            return &m_ubos.at(index);
        }
        FVertexBuffer* getVBO(size_t index) final {
            return &m_vbos.at(index);
        }
        FIndexBuffer* getIBO(size_t index) final {
            return &m_ibos.at(index);
        }
        FShaderPipeline* getShaderPipeline(size_t index) final {
            return &m_shaderPipelines.at(index);
        }
        FGraphicsPipelineColorMesh* getPipelineColorMesh(size_t index) final {
            return &m_pipelinesColorMesh.at(index);
        }
        FGraphicsPipelineTexture2DMesh* getPipelineTexture2DMesh(size_t index) final {
            return &m_pipelinesTexture2DMesh.at(index);
        }

        FRenderer2* getRenderer() final {
            return &m_renderer;
        }

        FGraphicsPipelineMesh* retrieveCorrectPipeline(EGraphicsPipelineType type,
                                                       size_t index) {
            if (type == EGraphicsPipelineType::MESH_COLOR) {
                return getPipelineColorMesh(index);
            }
            if (type == EGraphicsPipelineType::MESH_TEXTURE2D) {
                return getPipelineTexture2DMesh(index);
            }

            return nullptr;
        }

        void reset() final {
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

    private:

        std::vector<FShaderStorageBufferOpenGL2> m_ssbos;
        std::vector<FUniformBufferOpenGL2> m_ubos;
        std::vector<FVertexBufferOpenGL> m_vbos;
        std::vector<FIndexBufferOpenGL> m_ibos;
        std::vector<FShaderPipelineOpenGL> m_shaderPipelines;
        std::vector<FGraphicsPipelineColorMeshOpenGL> m_pipelinesColorMesh;
        std::vector<FGraphicsPipelineTexture2DMeshOpenGL> m_pipelinesTexture2DMesh;
        FRendererOpenGL m_renderer;

    };


    class FGraphicsContextOpenGL : public FGraphicsContext {
    public:

        bool create(FWindow* pWindow) final;

        void close() final;

        void prepareFrame() final;

        void endFrame() final;

        EGraphicsContextType getType() const final;

        FGraphicsFactory* getFactory() final;

    private:

        FGraphicsFactoryOpenGL m_factory;
        FWindow* m_pWindow{ nullptr };

    };


}



#endif //MARENGINE_GRAPHICSCONTEXTOPENGL_H
