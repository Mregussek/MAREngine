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


#include "../../mar.h"


namespace marengine {

    class FWindow;


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

    constexpr uint32_t g_MeshStride{ 3 + 3 + 2 + 1 };

    namespace GraphicLimits {

        constexpr uint32_t maxTrianglesCount{ 100000 };
        constexpr uint32_t maxVerticesCount{ maxTrianglesCount * 3 };
        constexpr uint32_t maxIndicesCount{ maxTrianglesCount * 3 };
        constexpr int64_t sizeOfVertices{ maxVerticesCount * sizeof(Vertex) };
        constexpr int64_t sizeOfIndices{ maxIndicesCount * sizeof(uint32_t) };

        constexpr uint32_t maxTransforms{ 32 };
        constexpr uint32_t maxLights{ 32 };

    };

    struct FTexturePair {
        FTexturePair(uint32_t bd, std::string tp) :
                bindingIndex(bd),
                texturePath(std::move(tp))
        {}

        uint32_t bindingIndex{ 0 };
        std::string texturePath{ "" };
    };

    typedef std::vector<Vertex> FVertexArray;
    typedef std::vector<uint32_t> FIndicesArray;
    typedef std::vector<maths::mat4> FTransformsArray;
    typedef std::vector<maths::vec4> FColorsArray;
    typedef std::vector<FTexturePair> FTexturesArray;


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

    class IRenderResource : public IRender {
    public:

        virtual void setIndex(int8 index) = 0;
        virtual int8 getIndex() const = 0;

    };

    class FRenderResource : public IRenderResource {
    public:

        void setIndex(int8 index) final { p_index = index; }
        MAR_NO_DISCARD int8 getIndex() const final { return p_index; }

    protected:

        int8 p_index{ -1 };

    };

    class IRenderResourceFactory : public IRenderFactory {

    };

    class IRenderResourceStorage : public IRenderStorage {
    public:

        virtual void reset() = 0;

    };

    class IRenderResourceManager : public IRenderManager {

    };

    class FBufferStorage;
    class FBufferFactory;
    class FShadersStorage;
    class FShadersFactory;
    class FPipelineStorage;
    class FPipelineFactory;

    class IRenderContext : public IRender {
    public:

        virtual bool create(FWindow* pWindow) = 0;
        virtual void close() = 0;

        virtual void prepareFrame() = 0;
        virtual void endFrame() = 0;

        virtual FBufferStorage* getBufferStorage() = 0;
        virtual FShadersStorage* getShadersStorage() = 0;
        virtual FPipelineStorage* getPipelineStorage() = 0;

        virtual FBufferFactory* getBufferFactory() = 0;
        virtual FShadersFactory* getShadersFactory() = 0;
        virtual FPipelineFactory* getPipelineFactory() = 0;

        virtual ERenderContextType getType() const = 0;

    };


    class FRenderContext : public IRenderContext {

    };


}


#endif //MARENGINE_IRENDER_H
