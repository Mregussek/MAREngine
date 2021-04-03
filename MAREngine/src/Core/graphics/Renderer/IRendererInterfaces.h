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


#ifndef MARENGINE_IRENDERERINTERFACES_H
#define MARENGINE_IRENDERERINTERFACES_H


#include "../../../mar.h"
#include "../Mesh/MeshDefinitions.h"


namespace marengine {

    class FWindow;


    enum class EGraphicsContextType {
        NONE, OPENGL, VULKAN
    };

    enum class EGraphicsPipelineType {
        NONE, MESH_COLOR, MESH_TEXTURE2D
    };

    enum class EBufferType {
        NONE, VERTEX, INDEX, SSBO, UBO
    };

    enum class EVertexInputType {
        NONE, FLOAT
    };

    struct FVertexLayoutElement {
        EVertexInputType inputType{ EVertexInputType::NONE };
        uint32_t count{ 0 };
    };

    typedef std::vector<FVertexLayoutElement> FVertexLayoutArray;



    class IGraphicsContext {
    public:

        virtual bool create(FWindow* pWindow) = 0;

        virtual void close() = 0;

        virtual void prepareFrame() = 0;

        virtual void endFrame() = 0;

        virtual EGraphicsContextType getType() const = 0;

    };
    


    class FGraphicsContext : public IGraphicsContext { };

    

    class IGraphicsPipeline {
    public:

    };



    class IRenderResource { };



    class IBuffer : public IRenderResource {
    public:

        virtual void create(int64_t memoryToAllocate, uint32_t bindingPoint) = 0;
        virtual void free() = 0;
        virtual void destroy() = 0;

        virtual void update(const float* data, size_t offset, size_t sizeOfData) = 0;
        virtual void update(const uint32_t* data, size_t offset, size_t sizeOfData) = 0;

    };



    class IShaderPipeline : public IRenderResource {
    public:

        virtual void passVertexShader(const char* vertexShader) = 0;
        virtual void passTesselationEvalShader(const char* tesselationEvalShader) = 0;
        virtual void passTesselationControlShader(const char* tesselationControlShader) = 0;
        virtual void passGeometryShader(const char* geometryShader) = 0;
        virtual void passComputeShader(const char* computeShader) = 0;
        virtual void passFragmentShader(const char* fragmentShader) = 0;

        virtual void compile() = 0;

    };


}


#endif //MARENGINE_IRENDERERINTERFACES_H
