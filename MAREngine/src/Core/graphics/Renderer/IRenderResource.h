


#ifndef MARENGINE_IRENDERERINTERFACES_H
#define MARENGINE_IRENDERERINTERFACES_H


#include "../../../mar.h"
#include "../Mesh/MeshDefinitions.h"


namespace marengine {


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



    class IMaterial : public IRenderResource {
    public:
    
    };


}


#endif //MARENGINE_IRENDERERINTERFACES_H
