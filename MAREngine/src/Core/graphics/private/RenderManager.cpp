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
#include "../public/BatchManager.h"
#include "../public/Buffer.h"
#include "../public/Material.h"
#include "../public/Framebuffer.h"
#include "../public/Shaders.h"
#include "../public/Pipeline.h"
#include "../public/RenderCamera.h"
#include "../public/MeshBatch.h"
#include "../public/LightBatch.h"


namespace marengine {


    void FRenderManager::create(FRenderContext* pContext) {
        m_pContext = pContext;
        m_pContext->getBufferFactory()->passRenderContext(pContext);
        m_pContext->getShadersFactory()->passRenderContext(pContext);
        m_pContext->getPipelineFactory()->passRenderContext(pContext);
        m_pContext->getMaterialFactory()->passRenderContext(pContext);

        FFramebufferFactory* pFbFactory{ m_pContext->getFramebufferFactory() };
        pFbFactory->passRenderContext(pContext);

        FFramebufferSpecification framebufferSpecs;
        framebufferSpecs.width = 800;
        framebufferSpecs.height = 600;

        FFramebuffer* viewportFramebuffer{ pFbFactory->emplace() };
        m_viewportFbIndex = viewportFramebuffer->getIndex();
        viewportFramebuffer->create(framebufferSpecs);
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

    void FRenderManager::setCamera(const FRenderCamera* pRenderCamera) {
        m_pRenderCamera = pRenderCamera;
    }

    void FRenderManager::pushCameraToRender(const FRenderCamera* pRenderCamera) {
        setCamera(pRenderCamera);
        const maths::mat4& mvp{ m_pRenderCamera->getMVP() };
        FShaderBuffer* const cameraSSBO{ m_pContext->getBufferStorage()->getSSBO(m_cameraIndex) };
        cameraSSBO->update(maths::mat4::value_ptr(mvp), 0, sizeof(maths::mat4));
    }

    bool FRenderManager::isCameraValid() const {
        return m_pRenderCamera != nullptr;
    }

    FFramebuffer* FRenderManager::getViewportFramebuffer() const {
        return m_pContext->getFramebufferStorage()->get(m_viewportFbIndex);
    }

    template<typename TMeshBatchStorage>
    static void preparePipelineForOnlyBindState() {

    }

    void FRenderManager::onBatchesReadyToDraw(FBatchManager* pBatchManager) {
        FBufferFactory* const pBufferFactory{ m_pContext->getBufferFactory() };
        FPipelineFactory* const pPipelineFactory{ m_pContext->getPipelineFactory() };

        FShaderBuffer* pCameraSSBO{ m_pContext->getBufferFactory()->emplaceSSBO() };
        m_cameraIndex = pBufferFactory->fillCameraSSBO(pCameraSSBO, m_pRenderCamera);

        FShaderBuffer* pPointLightSSBO{ m_pContext->getBufferFactory()->emplaceSSBO() };
        FPointLightBatch* pLightBatch{ pBatchManager->getLightBatchStorage()->getPointLightBatch() };
        m_pointLightIndex = pBufferFactory->fillPointLightSSBO(pPointLightSSBO,
                                                               pLightBatch);
        pLightBatch->passLightSSBO(m_pointLightIndex);

        FMeshBatchStorageStaticColor* pStorageStaticColor =
                pBatchManager->getMeshBatchStorage()->getStorageStaticColor();
        const uint32 colorBatchSize{ pStorageStaticColor->getCount() };
        for (uint32 i = 0; i < colorBatchSize; i++) {
            FPipelineMeshColor* pPipeline{ pPipelineFactory->emplaceMeshColor() };
            pPipelineFactory->fillPipelineFor(pPipeline, pStorageStaticColor->get(i));
            pPipeline->passCameraSSBO(m_cameraIndex);
            pPipeline->passPointLightSSBO(m_pointLightIndex);
        }
    }


}
