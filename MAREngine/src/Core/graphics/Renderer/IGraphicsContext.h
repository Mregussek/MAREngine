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


#ifndef MARENGINE_IGRAPHICSCONTEXT_H
#define MARENGINE_IGRAPHICSCONTEXT_H


namespace marengine {

    class FWindow;
    class FShaderPipeline;
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

    template<typename TEmplacedType>
    struct FContextEmplacedInfo {
        TEmplacedType* pType{ nullptr };
        size_t index{ 0 };
    };



    class IGraphics { };
    class IGraphicsManager : public IGraphics { };

    class IGraphicsFactory : public IGraphics { 
    public:

        // TODO: THIS IS HORRIBLE! need to learn how to implement it better in multi inheritance

        virtual FShaderBuffer* emplaceSSBO() = 0;
        virtual FShaderBuffer* emplaceUBO() = 0;
        virtual FVertexBuffer* emplaceVBO() = 0;
        virtual FIndexBuffer* emplaceIBO() = 0;
        virtual FShaderPipeline* emplaceShaderPipeline() = 0;
        virtual FGraphicsPipelineColorMesh* emplacePipelineColorMesh() = 0;
        virtual FGraphicsPipelineTexture2DMesh* emplacePipelineTexture2DMesh() = 0;
        
        virtual size_t getCountSSBO() const = 0;
        virtual size_t getCountUBO() const = 0;
        virtual size_t getCountVBO() const = 0;
        virtual size_t getCountIBO() const = 0;
        virtual size_t getCountShaderPipeline() const = 0;
        virtual size_t getCountPipelineColorMesh() const = 0;
        virtual size_t getCountPipelineTexture2DMesh() const = 0;

        virtual FShaderBuffer* getSSBO(size_t index) = 0;
        virtual FShaderBuffer* getUBO(size_t index) = 0;
        virtual FVertexBuffer* getVBO(size_t index) = 0;
        virtual FIndexBuffer* getIBO(size_t index) = 0;
        virtual FShaderPipeline* getShaderPipeline(size_t index) = 0;
        virtual FGraphicsPipelineColorMesh* getPipelineColorMesh(size_t index) = 0;
        virtual FGraphicsPipelineTexture2DMesh* getPipelineTexture2DMesh(size_t index) = 0;

        virtual FRenderer2* getRenderer() = 0;

        virtual FGraphicsPipelineMesh* retrieveCorrectPipeline(EGraphicsPipelineType type,
                                                               size_t index) = 0;

        virtual void reset() = 0;

    };


    class FGraphicsFactory : public IGraphicsFactory { };


    class IGraphicsContext : public IGraphics {
    public:

        virtual bool create(FWindow* pWindow) = 0;

        virtual void close() = 0;

        virtual void prepareFrame() = 0;

        virtual void endFrame() = 0;

        virtual EGraphicsContextType getType() const = 0;

        virtual FGraphicsFactory* getFactory() = 0;

    };


    class FGraphicsContext : public IGraphicsContext { };


    


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


#endif //MARENGINE_IGRAPHICSCONTEXT_H
