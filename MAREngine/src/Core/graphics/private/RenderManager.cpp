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
#include "../../../Logging/Logger.h"


namespace marengine {


    uint32 FRenderMode::s_renderMode{ GL_TRIANGLES };

    void FRenderMode::setTriangles() { s_renderMode = GL_TRIANGLES; }
    void FRenderMode::setLines() { s_renderMode = GL_LINES; }
    uint32 FRenderMode::getMode() { return s_renderMode; }


    void FRenderManager::create(FRenderContext* pContext) {
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Creating RenderManager...");
        m_pContext = pContext;

        FFramebufferSpecification framebufferSpecs;
        framebufferSpecs.width = 800;
        framebufferSpecs.height = 600;

        FFramebuffer* viewportFramebuffer{ m_pContext->getFramebufferFactory()->emplace() };
        m_viewportFbIndex = viewportFramebuffer->getIndex();
        viewportFramebuffer->create(framebufferSpecs);
        MARLOG_INFO(ELoggerType::GRAPHICS, "Created RenderManager!");
    }

    template<typename TMeshBatch>
    static void clearMeshBatch(TMeshBatch& meshesBatch) {
        for (auto& batch : meshesBatch) { batch.reset(); }
        meshesBatch.clear();
    }

    void FRenderManager::reset() {
        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Resetting RenderManager...");
		m_pContext->getBufferStorage()->reset();
		m_pContext->getPipelineStorage()->reset();
		m_pContext->getShadersStorage()->reset();
    }

    void FRenderManager::setCamera(const FRenderCamera* pRenderCamera) {
        m_pRenderCamera = pRenderCamera;
    }

    void FRenderManager::pushCameraToRender(const FRenderCamera* pRenderCamera) {
        //MARLOG_TRACE(ELoggerType::GRAPHICS, "Pushing given RenderCamera to rendering...");
        setCamera(pRenderCamera);
        const maths::mat4& mvp{ m_pRenderCamera->getMVP() };
        FShaderBuffer* const cameraSSBO{ m_pContext->getBufferStorage()->getSSBO(m_cameraIndex) };
        cameraSSBO->update(maths::mat4::value_ptr(mvp), 0, sizeof(maths::mat4));
        //MARLOG_DEBUG(ELoggerType::GRAPHICS, "Pushed given RenderCamera to rendering!");
    }

    bool FRenderManager::isCameraValid() const {
        return m_pRenderCamera != nullptr;
    }

    FFramebuffer* FRenderManager::getViewportFramebuffer() const {
        return m_pContext->getFramebufferStorage()->get(m_viewportFbIndex);
    }

    template<typename TMeshBatchStorage>
    static void preparePipelineForOnlyBindState(TMeshBatchStorage* pStorage,
                                                FPipelineFactory* pPipelineFactory,
                                                int32 cameraIndex, int32 pointLightIndex) {
        const uint32 batchSize{ pStorage->getCount() };

        for (uint32 i = 0; i < batchSize; i++) {
            auto* pMeshBatch{ pStorage->get(i) };
            if(pMeshBatch->getVertices().empty() || pMeshBatch->getIndices().empty()) {
                continue;
            }
            auto* pPipeline{ pPipelineFactory->emplaceMeshAndFill(pMeshBatch) };
            pPipeline->passCameraSSBO(cameraIndex);
            pPipeline->passPointLightSSBO(pointLightIndex);
        }
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

        FMeshBatchStorage* pMeshBatchStorage{ pBatchManager->getMeshBatchStorage() };
        preparePipelineForOnlyBindState(pMeshBatchStorage->getStorageStaticColor(),
                                        pPipelineFactory, m_cameraIndex, m_pointLightIndex);
        preparePipelineForOnlyBindState(pMeshBatchStorage->getStorageStaticTex2D(),
                                        pPipelineFactory, m_cameraIndex, m_pointLightIndex);
    }


}
