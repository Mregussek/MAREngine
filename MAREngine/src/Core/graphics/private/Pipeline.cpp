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


#include "../public/Pipeline.h"
#include "../public/Buffer.h"


namespace marengine {


    void FPipeline::passBufferStorage(FBufferStorage* pStorage) {
        p_pBufferStorage = pStorage;
    }

    void FPipeline::passShadersStorage(FShadersStorage* pStorage) {
        p_pShadersStorage = pStorage;
    }


    void FPipelineMesh::passVertexBuffer(int32 i) {
        p_vboIndex = i;
    }

    void FPipelineMesh::passIndexBuffer(int32 i) {
        p_iboIndex = i;
    }

    void FPipelineMesh::passTransformSSBO(int32 i) {
        p_transformIndex = i;
    }

    void FPipelineMesh::passCameraSSBO(int32 i) {
        p_camIndex = i;
    }

     void FPipelineMesh::passPointLightSSBO(int32 i) {
         p_pointLightIndex = i;
    }

    void FPipelineMesh::passShaderPipeline(int32 i) {
        p_shaderIndex = i;
    }

    uint32_t FPipelineMesh::getIndicesCount() const {
        return p_pBufferStorage->getIBO(p_iboIndex)->getIndicesCount();
    }


    void FPipelineMeshColor::passColorSSBO(int32 i) {
        p_colorIndex = i;
    }


}
