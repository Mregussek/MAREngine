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


	class FRenderCommand : public IRenderCommand {
	public:

	};


	struct FRenderStatsStorage {

        uint32_t drawCallsCount{ 0 };
        uint32_t verticesCount{ 0 };
        uint32_t indicesCount{ 0 };
        uint32_t trianglesCount{ 0 };
        uint32_t entitiesCount{ 0 };
        uint32_t coloredEntitiesCount{ 0 };
        uint32_t textured2dEntitiesCount{ 0 };
        uint32_t allRendererEntitiesCount{ 0 };

	};


    class FRenderStatistics {
    public:

        void update(FSceneManagerEditor* pSceneManagerEditor);
        void reset();

        MAR_NO_DISCARD const FRenderStatsStorage& getStorage() const;

    private:

        FRenderStatsStorage m_storage;

    };


}

#endif // !MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
