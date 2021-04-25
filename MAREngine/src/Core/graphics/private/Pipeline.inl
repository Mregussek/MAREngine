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


#ifndef MARENGINE_FPIPELINE_INL
#define MARENGINE_FPIPELINE_INL


#include "../public/Pipeline.h"
#include "MeshBatch.inl"


namespace marengine {


    template<typename TMeshBatch>
    FPipelineMesh* FPipelineFactory::emplaceMeshAndFill(TMeshBatch* pMeshBatch) {
        if constexpr (std::is_same_v<TMeshBatch, FMeshBatchStaticColor>) {
            FPipelineMeshColor* pPipeline{ emplaceMeshColor() };
            fillPipelineFor(pPipeline, pMeshBatch);
            return pPipeline;
        }
        else if constexpr (std::is_same_v<TMeshBatch, FMeshBatchStaticTex2D>) {
            FPipelineMeshTex2D* pPipeline{ emplaceMeshTex2D() };
            fillPipelineFor(pPipeline, pMeshBatch);
            return pPipeline;
        }

        return nullptr;
    }


}


#endif //MARENGINE_FPIPELINE_INL
