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


#ifndef MARENGINE_IRENDERRESOURCE_H
#define MARENGINE_IRENDERRESOURCE_H


#include "IRender.h"


namespace marengine {

    class FRenderContext;


    class IRenderResource : public IRender {
    public:

        virtual void setIndex(int32 index) = 0;
        virtual int32 getIndex() const = 0;

        virtual void setAssetID(uint32 id) = 0;
        virtual uint32 getAssetID() const = 0;

    };


    class FRenderResource : public IRenderResource {
    public:

        void setIndex(int32 index) final { p_index = index; }
        MAR_NO_DISCARD int32 getIndex() const final { return p_index; }

        void setAssetID(uint32 id) final { p_id = id; }
        MAR_NO_DISCARD uint32 getAssetID() const final { return p_id; }

    protected:

        int32 p_index{ -1 };
        uint32 p_id{ 0 };

    };


    class IRenderResourceFactory : public IRenderFactory {
    public:

        virtual void passRenderContext(FRenderContext* pRenderContext) = 0;

    };


    class FRenderResourceFactory : public IRenderResourceFactory {
    public:

        void passRenderContext(FRenderContext* pRenderContext) final { p_pRenderContext = pRenderContext; }

    protected:

        FRenderContext* p_pRenderContext{ nullptr };

    };


    class IRenderResourceStorage : public IRenderStorage {
    public:

        virtual void reset() = 0;

    };


    class IRenderResourceManager : public IRenderManager {

    };


}



#endif //MARENGINE_IRENDERRESOURCE_H
