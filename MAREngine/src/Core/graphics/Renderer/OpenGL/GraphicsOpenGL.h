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


#ifndef MARENGINE_GRAPHICSOPENGL_H
#define MARENGINE_GRAPHICSOPENGL_H


#include "../../../../mar.h"
#include "../IGraphics.h"
#include "RendererOpenGL.h"
#include "BufferOpenGL.h"
#include "GraphicsPipelineOpenGL.h"
#include "ShaderPipelineOpenGL.h"


namespace marengine {

    class FWindow;


    class FGraphicsFactoryOpenGL : public FGraphicsFactory {
    public:

        MAR_NO_DISCARD FShaderBuffer* const emplaceSSBO() final;
        MAR_NO_DISCARD FShaderBuffer* const emplaceUBO() final;
        MAR_NO_DISCARD FVertexBuffer* const emplaceVBO() final;
        MAR_NO_DISCARD FIndexBuffer* const emplaceIBO() final;
        MAR_NO_DISCARD FShaderPipeline* const emplaceShaderPipeline() final;
        MAR_NO_DISCARD FGraphicsPipelineColorMesh* const emplacePipelineColorMesh() final;
        MAR_NO_DISCARD FGraphicsPipelineTexture2DMesh* const emplacePipelineTexture2DMesh() final;

        MAR_NO_DISCARD size_t getCountSSBO() const final;
        MAR_NO_DISCARD size_t getCountUBO() const final;
        MAR_NO_DISCARD size_t getCountVBO() const final;
        MAR_NO_DISCARD size_t getCountIBO() const final;
        MAR_NO_DISCARD size_t getCountShaderPipeline() const final;
        MAR_NO_DISCARD size_t getCountPipelineColorMesh() const final;
        MAR_NO_DISCARD size_t getCountPipelineTexture2DMesh() const final;

        MAR_NO_DISCARD FShaderBuffer* const getSSBO(size_t index) const final;
        MAR_NO_DISCARD FShaderBuffer* const getUBO(size_t index) const final;
        MAR_NO_DISCARD FVertexBuffer* const getVBO(size_t index) const final;
        MAR_NO_DISCARD FIndexBuffer* const getIBO(size_t index) const final;
        MAR_NO_DISCARD FShaderPipeline* const getShaderPipeline(size_t index) const final;
        MAR_NO_DISCARD FGraphicsPipelineColorMesh* const getPipelineColorMesh(size_t index) const final;
        MAR_NO_DISCARD FGraphicsPipelineTexture2DMesh* const getPipelineTexture2DMesh(size_t index) const final;
        MAR_NO_DISCARD FRenderer2* const getRenderer() const final;

        MAR_NO_DISCARD FGraphicsPipelineMesh* const retrieveCorrectPipeline(EGraphicsPipelineType type,
                                                                            size_t index) const final;

        void reset() final;

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

        MAR_NO_DISCARD EGraphicsContextType getType() const final;
        MAR_NO_DISCARD FGraphicsFactory* getFactory() final;

    private:

        FGraphicsFactoryOpenGL m_factory;
        FWindow* m_pWindow{ nullptr };

    };


}



#endif //MARENGINE_GRAPHICSOPENGL_H
