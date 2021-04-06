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


#ifndef MARENGINE_IGRAPHICS_H
#define MARENGINE_IGRAPHICS_H


namespace marengine {

    class FWindow;
    class FShaderBuffer;
    class FVertexBuffer;
    class FIndexBuffer;
    class FShaderPipeline;
    class FGraphicsPipelineMesh;
    class FGraphicsPipelineColorMesh;
    class FGraphicsPipelineTexture2DMesh;
    class FRenderer2;


    enum class EGraphicsContextType {
        NONE, OPENGL, VULKAN
    };

    enum class EGraphicsPipelineType {
        NONE, MESH_COLOR, MESH_TEXTURE2D
    };


    class IGraphics {

    };

    class IGraphicsManager : public IGraphics {

    };

    class IGraphicsFactory : public IGraphics { 
    public:

        // TODO: THIS IS HORRIBLE! need to learn how to implement it better in multi inheritance

        virtual FShaderBuffer* const emplaceSSBO() = 0;
        virtual FShaderBuffer* const emplaceUBO() = 0;
        virtual FVertexBuffer* const emplaceVBO() = 0;
        virtual FIndexBuffer* const emplaceIBO() = 0;
        virtual FShaderPipeline* const emplaceShaderPipeline() = 0;
        virtual FGraphicsPipelineColorMesh* const emplacePipelineColorMesh() = 0;
        virtual FGraphicsPipelineTexture2DMesh* const emplacePipelineTexture2DMesh() = 0;
        
        virtual size_t getCountSSBO() const = 0;
        virtual size_t getCountUBO() const = 0;
        virtual size_t getCountVBO() const = 0;
        virtual size_t getCountIBO() const = 0;
        virtual size_t getCountShaderPipeline() const = 0;
        virtual size_t getCountPipelineColorMesh() const = 0;
        virtual size_t getCountPipelineTexture2DMesh() const = 0;

        virtual FShaderBuffer* const getSSBO(size_t index) const = 0;
        virtual FShaderBuffer* const getUBO(size_t index) const = 0;
        virtual FVertexBuffer* const getVBO(size_t index) const = 0;
        virtual FIndexBuffer* const getIBO(size_t index) const = 0;
        virtual FShaderPipeline* const getShaderPipeline(size_t index) const = 0;
        virtual FGraphicsPipelineColorMesh* const getPipelineColorMesh(size_t index) const = 0;
        virtual FGraphicsPipelineTexture2DMesh* const getPipelineTexture2DMesh(size_t index) const = 0;
        virtual FRenderer2* const getRenderer() const = 0;

        virtual FGraphicsPipelineMesh* const retrieveCorrectPipeline(EGraphicsPipelineType type,
                                                                     size_t index) const = 0;

        virtual void reset() = 0;

    };

    class FGraphicsFactory : public IGraphicsFactory {

    };

    class IGraphicsContext : public IGraphics {
    public:

        virtual bool create(FWindow* pWindow) = 0;
        virtual void close() = 0;

        virtual void prepareFrame() = 0;
        virtual void endFrame() = 0;

        virtual EGraphicsContextType getType() const = 0;
        virtual FGraphicsFactory* getFactory() = 0;

    };

    class FGraphicsContext : public IGraphicsContext {

    };

    class IGraphicsPipeline : public IGraphics {
    public:

        virtual void passFactory(FGraphicsFactory* pFactory) = 0;
        virtual void create() = 0;
        virtual void close() = 0;
        virtual void bind() = 0;

    };

    class IRenderer : public IGraphics {

    };

    class IRenderManager : public IGraphicsManager {

    };


}


#endif //MARENGINE_IGRAPHICS_H
