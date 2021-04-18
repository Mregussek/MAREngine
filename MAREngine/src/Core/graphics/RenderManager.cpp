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


#include "RenderManager.h"
#include "BatchManager.h"
#include "Buffer.h"
#include "Shaders.h"
#include "Pipeline.h"
#include "RenderCamera.h"
#include "MeshBatch.h"


namespace marengine {


    void FRenderManager::create(FRenderContext* pContext) {
        m_pContext = pContext;
        m_pContext->getBufferFactory()->passRenderContext(pContext);
        m_pContext->getShadersFactory()->passRenderContext(pContext);
        m_pContext->getPipelineFactory()->passRenderContext(pContext);
    }

    template<typename TMeshBatch>
    static void clearMeshBatch(TMeshBatch& meshesBatch) {
        for (auto& batch : meshesBatch) { batch.reset(); }
        meshesBatch.clear();
    }

    void FRenderManager::reset() {
		m_pContext->getBufferStorage()->reset();
		m_pContext->getPipelineStorage()->reset();
		m_pContext->getShadersStorage()->reset();
    }

    void FRenderManager::setCamera(const RenderCamera* pRenderCamera) {
        m_pRenderCamera = pRenderCamera;
    }

    void FRenderManager::pushCameraToRender(const RenderCamera* pRenderCamera) {
        setCamera(pRenderCamera);
        const maths::mat4& mvp{ m_pRenderCamera->getMVP() };
        FShaderBuffer* const cameraSSBO{ m_pContext->getBufferStorage()->getSSBO(m_cameraIndex) };
        cameraSSBO->update(maths::mat4::value_ptr(mvp), 0, sizeof(maths::mat4));
    }

    bool FRenderManager::isCameraValid() const {
        return m_pRenderCamera != nullptr;
    }

    void FRenderManager::onBatchesReadyToDraw(FBatchManager* pBatchManager) {
        FBufferFactory* const pBufferFactory{ m_pContext->getBufferFactory() };
        FPipelineFactory* const pPipelineFactory{ m_pContext->getPipelineFactory() };

        FShaderBuffer* pCameraSSBO{ m_pContext->getBufferFactory()->emplaceSSBO() };
        m_cameraIndex = pBufferFactory->fillCameraSSBO(pCameraSSBO, m_pRenderCamera);

        FShaderBuffer* pPointLightSSBO{ m_pContext->getBufferFactory()->emplaceSSBO() };
        m_pointLightIndex = pBufferFactory->fillPointLightSSBO(pPointLightSSBO,
                                                               pBatchManager->getPointLightBatch());

        FMeshBatchStorage* pStorage{ pBatchManager->getMeshBatchStorage() };
        const int8 colorBatchSize{ (int8)pStorage->getCountStaticColor() };

        for (int8 i = 0; i < colorBatchSize; i++) {
            FPipelineMeshColor* pPipeline{ pPipelineFactory->emplaceColorMesh() };
            pPipelineFactory->fillPipelineFor(pPipeline, pStorage->getStaticColor(i));
            pPipeline->passCameraSSBO(m_cameraIndex);
            pPipeline->passPointLightSSBO(m_pointLightIndex);
        }
    }


}
