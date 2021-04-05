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



    class IGraphicsFactory { };
    class IGraphics { };
    class IGraphicsManager : public IGraphics { };


    class IGraphicsContextFactory : public IGraphicsFactory {
    public:

        virtual FContextEmplacedInfo<FShaderBuffer> emplaceShaderStorageBuffer() = 0;
        virtual FContextEmplacedInfo<FShaderBuffer> emplaceUniformBuffer() = 0;
        virtual FContextEmplacedInfo<FVertexBuffer> emplaceVertexBuffer() = 0;
        virtual FContextEmplacedInfo<FIndexBuffer> emplaceIndexBuffer() = 0;
        virtual FContextEmplacedInfo<FShaderPipeline> emplaceShaderPipeline() = 0;
        virtual FContextEmplacedInfo<FGraphicsPipelineColorMesh> emplaceGraphicsPipelineColorMesh() = 0;
        virtual FContextEmplacedInfo<FGraphicsPipelineTexture2DMesh> emplaceGraphicsPipelineTexture2DMesh() = 0;
        virtual FRenderer2* emplaceRenderer() = 0;

        virtual FShaderBuffer* retrieveSSBO(size_t index) = 0;
        virtual FShaderBuffer* retrieveUBO(size_t index) = 0;
        virtual FVertexBuffer* retrieveVertexBuffer(size_t index) = 0;
        virtual FIndexBuffer* retrieveIndexBuffer(size_t index) = 0;
        virtual FShaderPipeline* retrieveShaderPipeline(size_t index) = 0;
        virtual FGraphicsPipelineColorMesh* retrieveGraphicsPipelineColorMesh(size_t index) = 0;
        virtual FGraphicsPipelineTexture2DMesh* retrieveGraphicsPipelineTexture2DMesh(size_t index) = 0;

    };


    class FGraphicsContextFactory : public IGraphicsContextFactory { };



    class IGraphicsContext : public IGraphics {
    public:

        virtual bool create(FWindow* pWindow) = 0;

        virtual void close() = 0;

        virtual void prepareFrame() = 0;

        virtual void endFrame() = 0;

        virtual EGraphicsContextType getType() const = 0;

        virtual FGraphicsContextFactory* getFactory() = 0;

    };


    class FGraphicsContext : public IGraphicsContext { };



    class IGraphicsPipeline : public IGraphics {
    public:

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
