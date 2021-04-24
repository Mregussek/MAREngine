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


#include "../public/Buffer.h"
#include "../public/RenderCamera.h"
#include "../public/LightBatch.h"


namespace marengine {


    const FVertexInputDescription& FVertexBuffer::getInputDescription() const {
        return p_inputDescription;
    }

    void FVertexBuffer::setInputDescription(const FVertexInputDescription& inputDescription) {
        p_inputDescription = inputDescription;
    }

    void FVertexBuffer::pushVariableInfo(const FVertexInputVariableInfo& info) {
        p_inputDescription.inputVariables.push_back(info);
    }


    void FIndexBuffer::passIndicesCount(uint32_t indicesCount) {
        p_indicesCount = indicesCount;
    }

    uint32_t FIndexBuffer::getIndicesCount() const {
        return p_indicesCount;
    }


    const FShaderInputDescription& FShaderBuffer::getInputDescription() const {
        return p_inputDescription;
    }

    void FShaderBuffer::setInputDescription(const FShaderInputDescription& inputDescription) {
        p_inputDescription = inputDescription;
    }

    void FShaderBuffer::pushVariableInfo(const FShaderInputVariableInfo& info) {
        p_inputDescription.inputVariables.push_back(info);
    }


    uint32 FBufferFactory::fillCameraSSBO(FShaderBuffer* pShaderBuffer,
                                          const RenderCamera* const pRenderCamera) const {
        FShaderInputDescription description;
        description.binding = 0;
        description.shaderStage = EShaderStage::VERTEX;
        description.bufferType = EBufferType::SSBO;

        FShaderInputVariableInfo mvpInfo;
        mvpInfo.count = 1;
        mvpInfo.inputType = EInputType::MAT4;
        mvpInfo.name = "Camera.MVP";
        mvpInfo.offset = 0;
        mvpInfo.memoryUsed = sizeof(maths::mat4);

        pShaderBuffer->setInputDescription(description);
        pShaderBuffer->pushVariableInfo(mvpInfo);

        const maths::mat4& mvp{ pRenderCamera->getMVP() };
        pShaderBuffer->create();
        pShaderBuffer->update(maths::mat4::value_ptr(mvp), mvpInfo.offset, sizeof(maths::mat4));

        return pShaderBuffer->getIndex();
    }


    uint32 FBufferFactory::fillPointLightSSBO(FShaderBuffer* pShaderBuffer,
                                              const FPointLightBatch* const pPointLightBatch) const {
        FShaderInputDescription description;
        description.binding = 2;
        description.shaderStage = EShaderStage::FRAGMENT;
        description.bufferType = EBufferType::SSBO;

        // TODO: change in shader code to proper PointLights...
        FShaderInputVariableInfo materialInfo;
        materialInfo.count = 32;
        materialInfo.inputType = EInputType::OTHER;
        materialInfo.name = "PointLights.LightMaterial[32]";
        materialInfo.offset = 0;
        materialInfo.memoryUsed = 32 * sizeof(FPointLight);

        FShaderInputVariableInfo sizeInfo;
        sizeInfo.count = 1;
        sizeInfo.inputType = EInputType::INT;
        sizeInfo.name = "PointLights.LightMaterialSize";
        sizeInfo.offset = materialInfo.memoryUsed;
        sizeInfo.memoryUsed = 1 * sizeof(int32_t);

        pShaderBuffer->setInputDescription(description);
        pShaderBuffer->pushVariableInfo(materialInfo);
        pShaderBuffer->pushVariableInfo(sizeInfo);

        const float* pData{ pPointLightBatch->getBatchData() };
        const uint32 lightSize{ pPointLightBatch->getCountLight() };
        pShaderBuffer->create();
        pShaderBuffer->update(pData,
                              materialInfo.offset,
                              lightSize * sizeof(FPointLight));
        pShaderBuffer->update(&lightSize,
                              sizeInfo.offset,
                              sizeof(uint32));

        return pShaderBuffer->getIndex();
    }


}
