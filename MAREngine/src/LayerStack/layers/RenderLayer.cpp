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


#include "RenderLayer.h"
#include "../../Core/filesystem/SceneDeserializer.h"


namespace marengine {


    FRenderStatistics* FRenderLayer::getRenderStats() const {
        return const_cast<FRenderStatistics*>(&p_statistics);
    }

    FBatchManager* FRenderLayer::getBatchManager() const {
        return const_cast<FBatchManager*>(&p_batchManager);
    }

    FRenderManager* FRenderLayer::getRenderManager() const {
        return const_cast<FRenderManager*>(&p_renderManager);
    }

    FMeshManager* FRenderLayer::getMeshManager() const {
        return const_cast<FMeshManager*>(&p_meshManager);
    }


    void FRenderLayerOpenGL::create(FWindow* pWindow) {
        m_context.create(pWindow);
        p_batchManager.create(&p_renderManager, &p_meshManager);
        p_renderManager.create((FRenderContext*)&m_context);
    }

    void FRenderLayerOpenGL::begin() {
        //m_context.prepareFrame();
    }

    void FRenderLayerOpenGL::update() {
        p_statistics.reset();

        const uint32_t colorMeshesCount{ m_context.getPipelineStorage()->getCountColorMesh() };

        for(uint32_t i = 0; i < colorMeshesCount; i++) {
            m_renderCmds.draw(m_context.getPipelineStorage()->getColorMesh(i));
        }
    }

    void FRenderLayerOpenGL::end() {

    }

    void FRenderLayerOpenGL::close() {

    }


}
