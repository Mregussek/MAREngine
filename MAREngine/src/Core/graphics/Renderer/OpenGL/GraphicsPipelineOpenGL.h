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


#ifndef MARENGINE_GRAPHICSPIPELINEOPENGL_H
#define MARENGINE_GRAPHICSPIPELINEOPENGL_H


#include "../IGraphicsPipeline.h"


namespace marengine {

    class FVertexBufferOpenGL;
    class FIndexBufferOpenGL;
    class FVertexShaderOpenGL;
    class FFragmentShaderOpenGL;
    class FTransformBufferOpenGL;
    class FColorBufferOpenGL;
    class FTextureSamplesOpenGL;
    class FCameraBufferOpenGL;


    class FGraphicsPipelineColorMeshOpenGL :
            public FGraphicsPipelineColorMesh<FGraphicsPipelineColorMeshOpenGL> {
    public:

        void passVertexShader(FVertexShaderOpenGL* pVertexShader);
        void passFragmentShader(FFragmentShaderOpenGL* pFragmentShader);
        void passVertexBuffer(FVertexBufferOpenGL* pVertexBuffer);
        void passIndexBuffer(FIndexBufferOpenGL* pIndexBuffer);
        void passTransformBuffer(FTransformBufferOpenGL* pTransformBuffer);
        void passCameraBuffer(FCameraBufferOpenGL* pCameraBuffer);
        void passColorBuffer(FColorBufferOpenGL* pColorBuffer);

        void create();
        void close();

    private:

        FVertexBufferOpenGL* m_pVertexShader{ nullptr };
        FIndexBufferOpenGL* m_pFragmentShader{ nullptr };
        FVertexShaderOpenGL* m_pVertexBuffer{ nullptr };
        FFragmentShaderOpenGL* m_pIndexBuffer{ nullptr };
        FCameraBufferOpenGL* m_pCameraBuffer{ nullptr };
        FTransformBufferOpenGL* m_pTransformBuffer{ nullptr };

        FColorBufferOpenGL* m_pColorBuffer{ nullptr };

    };


    class FGraphicsPipelineTexture2DMeshOpenGL :
            public FGraphicsPipelineTexture2DMesh<FGraphicsPipelineTexture2DMeshOpenGL> {
    public:

        void passVertexShader(FVertexShaderOpenGL* pVertexShader);
        void passFragmentShader(FFragmentShaderOpenGL* pFragmentShader);
        void passVertexBuffer(FVertexBufferOpenGL* pVertexBuffer);
        void passIndexBuffer(FIndexBufferOpenGL* pIndexBuffer);
        void passTransformBuffer(FTransformBufferOpenGL* pTransformBuffer);
        void passCameraBuffer(FCameraBufferOpenGL* pCameraBuffer);
        void passTextureSamples(FTextureSamplesOpenGL* pTextureSamples);

        void create();
        void close();

    private:

        FVertexBufferOpenGL* m_pVertexShader{ nullptr };
        FIndexBufferOpenGL* m_pFragmentShader{ nullptr };
        FVertexShaderOpenGL* m_pVertexBuffer{ nullptr };
        FFragmentShaderOpenGL* m_pIndexBuffer{ nullptr };
        FCameraBufferOpenGL* m_pCameraBuffer{ nullptr };
        FTransformBufferOpenGL* m_pTransformBuffer{ nullptr };

        FTextureSamplesOpenGL* m_pTextureSamples{ nullptr };

    };


}


#endif //MARENGINE_GRAPHICSPIPELINEOPENGL_H
