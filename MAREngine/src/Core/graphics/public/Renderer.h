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


#ifndef MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
#define MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H


#include "IRender.h"


namespace marengine {

    class FSceneManagerEditor;
    class FBatchManager;
    class FRenderStatistics;



	class FRenderCommand : public IRenderCommand {
	public:

        void create(FRenderStatistics* pRenderStatistics);

    protected:

        FRenderStatistics* p_pRenderStatistics{ nullptr };

	};


	struct FRenderStatsStorage {
        uint32 drawCallsCount{ 0 };
        uint32 verticesCount{ 0 };
        uint32 indicesCount{ 0 };
        uint32 trianglesCount{ 0 };
        uint32 entitiesCount{ 0 };
        uint32 coloredEntitiesCount{ 0 };
        uint32 textured2dEntitiesCount{ 0 };
        uint32 allRendererEntitiesCount{ 0 };
	};


    class FRenderStatistics {
    public:

        void create(FBatchManager* pBatchManager);
        void update(FSceneManagerEditor* pSceneManagerEditor);
        void reset();

        MAR_NO_DISCARD FRenderStatsStorage& getStorage();

    private:

        FRenderStatsStorage m_storage;
        FBatchManager* m_pBatchManager{ nullptr };

    };


}

#endif // !MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
