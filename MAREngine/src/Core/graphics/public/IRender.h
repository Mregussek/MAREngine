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


#ifndef MARENGINE_IRENDER_H
#define MARENGINE_IRENDER_H


#include "../../../mar.h"


namespace marengine {

    class FWindow;
    class FBufferStorage;
    class FBufferFactory;
    class FShadersStorage;
    class FShadersFactory;
    class FPipelineStorage;
    class FPipelineFactory;
    class FFramebufferFactory;
    class FFramebufferStorage;
    class FMaterialFactory;
    class FMaterialStorage;


    enum class ERenderContextType {
        NONE, OPENGL, VULKAN
    };

    enum class ERenderPipelineType {
        NONE, MESH_COLOR, MESH_TEXTURE2D
    };

    enum class EBufferType {
        NONE, VERTEX, INDEX, SSBO, UBO
    };

    enum class EInputType {
        NONE, FLOAT, INT, VEC4, VEC3, VEC2, MAT4, OTHER
    };

    enum class EShaderStage {
        NONE, VERTEX, FRAGMENT, TESS_EVAL, TESS_CONTROL, COMPUTE, GEOMETRY
    };

    /**
	 * @struct Vertex Vertex.h "Core/graphics/Mesh/Vertex.h"
	 * @brief Structure defining Vertex in MAREngine.
	 * Contains position (3 floats), lightNormal (3 floats),
	 * textureCoordinates (2 floats) and shapeID (1 float).
	 */
    struct Vertex {
        maths::vec3 position;
        maths::vec3 lightNormal;
        maths::vec2 textureCoordinates;
        float shapeID{ 0.f };
    };

    constexpr uint32 g_MeshStride{ 3 + 3 + 2 + 1 };

    namespace GraphicLimits {

        constexpr uint32 maxTrianglesCount{ 100000 };
        constexpr uint32 maxVerticesCount{ maxTrianglesCount * 3 };
        constexpr uint32 maxIndicesCount{ maxTrianglesCount * 3 };
        constexpr int64 sizeOfVertices{ maxVerticesCount * sizeof(Vertex) };
        constexpr int64 sizeOfIndices{ maxIndicesCount * sizeof(uint32_t) };

        constexpr uint32 maxTransforms{ 32 };
        constexpr uint32 maxLights{ 32 };

    };

    typedef std::vector<Vertex> FVertexArray;
    typedef std::vector<uint32> FIndicesArray;
    typedef std::vector<maths::mat4> FTransformsArray;
    typedef std::vector<maths::vec4> FColorsArray;


    class IRender {

    };

    class IRenderFactory : public IRender {

    };

    class IRenderManager : public IRender {

    };

    class IRenderCommand : public IRender {

    };

    class IRenderStorage : public IRender {

    };

    class IRenderContext : public IRender {
    public:

        virtual bool create(FWindow* pWindow) = 0;
        virtual void close() = 0;

        virtual void prepareFrame() = 0;
        virtual void endFrame() = 0;

        virtual FBufferStorage* getBufferStorage() const = 0;
        virtual FShadersStorage* getShadersStorage() const = 0;
        virtual FPipelineStorage* getPipelineStorage() const = 0;
        virtual FFramebufferStorage* getFramebufferStorage() const = 0;
        virtual FMaterialStorage* getMaterialStorage() const = 0;

        virtual FBufferFactory* getBufferFactory() const = 0;
        virtual FShadersFactory* getShadersFactory() const = 0;
        virtual FPipelineFactory* getPipelineFactory() const = 0;
        virtual FFramebufferFactory* getFramebufferFactory() const = 0;
        virtual FMaterialFactory* getMaterialFactory() const = 0;

        virtual ERenderContextType getType() const = 0;

    };


    class FRenderContext : public IRenderContext {

    };


}


#endif //MARENGINE_IRENDER_H
