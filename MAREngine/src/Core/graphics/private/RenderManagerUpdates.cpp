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


#include "../public/RenderManager.h"
#include "../public/MeshBatch.h"
#include "../public/LightBatch.h"
#include "OpenGL/GraphicsOpenGL.h"


namespace marengine {


    template<>
    void FRenderManager::update<ERenderBatchUpdateType::TRANSFORM>(FMeshBatch* pBatch) const {
        FShaderBuffer* pShaderBuffer =
                m_pContext->getBufferStorage()->getSSBO(pBatch->getTransformSSBO());

        const FTransformsArray& transforms{ pBatch->getTransforms() };
        pShaderBuffer->update(
                maths::mat4::value_ptr(transforms),
                0,
                transforms.size() * sizeof(maths::mat4)
        );
    }

    template<>
    void FRenderManager::update<ERenderBatchUpdateType::POINTLIGHT>(
            FPointLightBatch* pBatch) const {

        FShaderBuffer* pShaderBuffer =
                m_pContext->getBufferStorage()->getSSBO(pBatch->getUniquePointLightID());

        const FPointLightsArray& lights{ pBatch->getLights() };
        const int32 lightSize{ (int32)lights.size() };
        const FShaderInputDescription& inputDescription{ pShaderBuffer->getInputDescription() };
        {
            const FShaderInputVariableInfo& inputInfo{ inputDescription.inputVariables.at(0) };
            pShaderBuffer->update(&lights.at(0).position.x,
                                  inputInfo.offset,
                                  inputInfo.typeSize() * lightSize);
        }
        {
            const FShaderInputVariableInfo& inputInfo{ inputDescription.inputVariables.at(1) };
            pShaderBuffer->update(&lightSize,
                                  inputInfo.offset,
                                  inputInfo.typeSize() * 1);
        }
    }

    template<>
    void FRenderManager::update<ERenderBatchUpdateType::RENDERABLE_COLOR>(
            FMeshBatchStaticColor* pBatch) const {

        FShaderBuffer* pShaderBuffer =
                m_pContext->getBufferStorage()->getSSBO(pBatch->getColorSSBO());

        const FColorsArray& colors{ pBatch->getColors() };
        pShaderBuffer->update(
                maths::vec4::value_ptr(colors),
                0,
                colors.size() * sizeof(maths::vec4)
        );
    }


}

