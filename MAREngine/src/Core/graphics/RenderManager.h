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


#ifndef MARENGINE_RENDERMANAGER_H
#define MARENGINE_RENDERMANAGER_H


#include "IRender.h"


namespace marengine {

    class FMeshBatch;
    class FMeshBatchStaticColor;
    class RenderCamera;
    class FBatchManager;


    enum class ERenderBatchUpdateType {
        NONE, TRANSFORM, RENDERABLE_COLOR
    };


    class FRenderManager : public IRenderManager {
    public:

        void create(FRenderContext* pContext);
        void reset();

        void setCamera(const RenderCamera* pRenderCamera);
        void pushCameraToRender(const RenderCamera* pRenderCamera);
        MAR_NO_DISCARD bool isCameraValid() const;
        void onBatchesReadyToDraw(FBatchManager* pBatchManager);

        template<ERenderBatchUpdateType TUpdateType, typename TMeshBatch>
        void update(TMeshBatch* pMeshBatch) const { }

    private:

        const RenderCamera* m_pRenderCamera{ nullptr };
        FRenderContext* m_pContext{ nullptr };

        int32_t m_cameraIndex{ -1 };
        int32_t m_pointLightIndex{ -1 };

    };


    template<>
    void FRenderManager::update<ERenderBatchUpdateType::TRANSFORM>(FMeshBatch* pMeshBatch) const;
    template<>
    void FRenderManager::update<ERenderBatchUpdateType::RENDERABLE_COLOR>(
            FMeshBatchStaticColor* pMeshBatch) const;


}


#endif //MARENGINE_RENDERMANAGER_H
