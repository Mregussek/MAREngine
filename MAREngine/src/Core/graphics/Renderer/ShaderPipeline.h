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


#ifndef MARENGINE_SHADER_H
#define MARENGINE_SHADER_H


#include "IRenderResource.h"


namespace marengine {


    class FShaderPipeline : public IShaderPipeline {
    public:

        void passVertexShader(const char* vertexShader) final;
        void passTesselationEvalShader(const char* tesselationEvalShader) final;
        void passTesselationControlShader(const char* tesselationControlShader) final;
        void passGeometryShader(const char* geometryShader) final;
        void passComputeShader(const char* computeShader) final;
        void passFragmentShader(const char* fragmentShader) final;
        void passInputDescription(const FShaderInputDescription& inputDescription) final;

    protected:

        FShaderInputDescription p_inputDescription{};
        const char* p_vertexShader{ nullptr };
        const char* p_fragmentShader{ nullptr };
        const char* p_geometryShader{ nullptr };
        const char* p_tesselationEvalShader{ nullptr };
        const char* p_tesselationControlShader{ nullptr };
        const char* p_computeShader{ nullptr };

    };


}


#endif //MARENGINE_SHADER_H
