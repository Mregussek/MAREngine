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


#ifndef MARENGINE_ILIGHTBATCH_H
#define MARENGINE_ILIGHTBATCH_H


#include "IRenderResource.h"


namespace marengine {

    class FPointLightBatch;
    class Entity;


    enum class ELightBatchType {
        NONE = -1,
        POINTLIGHT = 1,
        DIRECTIONALLIGHT = 2
    };


    class ILightBatch : public FRenderResource {
    public:

        virtual const float* getBatchData() const = 0;
        virtual uint32 getCountLight() const = 0;

        virtual bool shouldBeBatched(const Entity& entity) const = 0;
        virtual bool canBeBatched(const Entity& entity) const = 0;
        virtual void submitToBatch(const Entity& entity) = 0;

        virtual void passLightSSBO(int32 index) = 0;
        virtual int32 getLightSSBO() const = 0;

        virtual ELightBatchType getType() const = 0;

    };


    class ILightBatchStorage : public IRenderResourceStorage {
    public:

        virtual FPointLightBatch* getPointLightBatch() const = 0;
        virtual uint32 getCountPointLightBatch() const = 0;

    };


    class ILightBatchFactory : public FRenderResourceFactory {
    public:

        virtual FPointLightBatch* emplacePointLightBatch() = 0;

    };


}


#endif //MARENGINE_ILIGHTBATCH_H
