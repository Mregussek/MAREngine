


#ifndef MARENGINE_IRENDERERINTERFACES_H
#define MARENGINE_IRENDERERINTERFACES_H


#include "../../../mar.h"
#include "../Mesh/MeshDefinitions.h"


namespace marengine {


    enum class EBufferType {
        NONE, VERTEX, INDEX, SSBO, UBO
    };

    enum class EBufferInputType {
        NONE, FLOAT, INT, VEC4, VEC3, VEC2, MAT4, OTHER
    };

    enum class EShaderStage {
        NONE, VERTEX, FRAGMENT, TESS_EVAL, TESS_CONTROL, COMPUTE, GEOMETRY
    };

    struct FVertexInputLayoutInfo {
        EBufferInputType inputType{ EBufferInputType::NONE };
        uint32_t location{ 0 };
        uint32_t offset{ 0 };
    };

    typedef std::vector<FVertexInputLayoutInfo> FVertexInputLayoutInfoArray;

    struct FVertexInputDescription {
        FVertexInputLayoutInfoArray layoutArray;
        uint32_t binding{ 0 };
        uint32_t stride{ 0 };
    };

    struct FShaderBufferItem {
        EBufferInputType inputType{ EBufferInputType::NONE };
        const char* name{ nullptr };
        uint32_t offset{ 0 };
        uint32_t memoryUsed{ 0 };
        uint32_t count{ 0 };
    };

    typedef std::vector<FShaderBufferItem> FShaderBufferItemsArray;

    struct FShaderInputLayoutInfo {
        FShaderBufferItemsArray items;
        EBufferType bufferType{ EBufferType::NONE };    // should be SSBO or UBO, other are ignored
        EShaderStage shaderStage{ EShaderStage::NONE };
        uint32_t binding{ 0 };
    };

    typedef std::vector<FShaderInputLayoutInfo> FShaderInputLayoutInfoArray;

    struct FShaderInputDescription {
        FShaderInputLayoutInfoArray* pInputLayoutInfoArray;
    };

    

    class IRenderResource { };



    class IBuffer : public IRenderResource {
    public:

        virtual void free() = 0;
        virtual void destroy() = 0;
        virtual void bind() = 0;

        virtual void update(const float* data, size_t offset, size_t sizeOfData) = 0;
        virtual void update(const uint32_t* data, size_t offset, size_t sizeOfData) = 0;
        virtual void update(const int32_t* data, size_t offset, size_t sizeOfData) = 0;

    };



    class IShaderPipeline : public IRenderResource {
    public:

        virtual void passVertexShader(const char* vertexShader) = 0;
        virtual void passTesselationEvalShader(const char* tesselationEvalShader) = 0;
        virtual void passTesselationControlShader(const char* tesselationControlShader) = 0;
        virtual void passGeometryShader(const char* geometryShader) = 0;
        virtual void passComputeShader(const char* computeShader) = 0;
        virtual void passFragmentShader(const char* fragmentShader) = 0;
        virtual void passInputDescription(const FShaderInputDescription& inputDescription) = 0;

        virtual void compile() = 0;
        virtual void close() = 0;
        virtual void bind() = 0;

    };



    class IMaterial : public IRenderResource {
    public:
    
    };


}


#endif //MARENGINE_IRENDERERINTERFACES_H
