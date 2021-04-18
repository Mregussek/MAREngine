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


#ifndef MARENGINE_SHADERS_H
#define MARENGINE_SHADERS_H


#include "IRender.h"


namespace marengine {


    struct FShaderInputVariableInfo {
        EInputType inputType{ EInputType::NONE };
        const char* name{ nullptr };
        uint32_t offset{ 0 };
        uint32_t memoryUsed{ 0 };
        uint32_t count{ 0 };

        uint32 typeSize() const { return memoryUsed / count; }

    };


    struct FShaderInputDescription {
        std::vector<FShaderInputVariableInfo> inputVariables;
        EBufferType bufferType{ EBufferType::NONE };
        EShaderStage shaderStage{ EShaderStage::NONE };
        uint32_t binding{ 0 };
    };


    class IShaders : public FRenderResource {
    public:

        virtual void passVertex(const char* vertexShader) = 0;
        virtual void passTessEval(const char* tesselationEvalShader) = 0;
        virtual void passTessControl(const char* tesselationControlShader) = 0;
        virtual void passGeometry(const char* geometryShader) = 0;
        virtual void passCompute(const char* computeShader) = 0;
        virtual void passFragment(const char* fragmentShader) = 0;

        virtual void compile() = 0;
        virtual void close() = 0;
        virtual void bind() = 0;

    };


    class FShaders : public IShaders {
    public:

        void passVertex(const char* path) final;
        void passTessEval(const char* path) final;
        void passTessControl(const char* path) final;
        void passGeometry(const char* path) final;
        void passCompute(const char* path) final;
        void passFragment(const char* path) final;

    protected:

        const char* p_vertexPath{ nullptr };
        const char* p_fragPath{ nullptr };
        const char* p_geometryPath{ nullptr };
        const char* p_tessEvalPath{ nullptr };
        const char* p_tessControlPath{ nullptr };
        const char* p_computePath{ nullptr };

    };


    class IShadersStorage : public IRenderResourceStorage {

    };


    class FShadersStorage : public IShadersStorage {
    public:

        virtual size_t getCount() const = 0;
        virtual FShaders* get(size_t index) const = 0;

    };


    class IShadersFactory : public FRenderResourceFactory {
    public:

        virtual FShaders* emplace() = 0;

    };


    class FShadersFactory : public IShadersFactory {

    };


}


#endif //MARENGINE_SHADERS_H
