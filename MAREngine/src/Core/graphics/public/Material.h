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


#ifndef MARENGINE_MATERIAL_H
#define MARENGINE_MATERIAL_H


#include "IRenderResource.h"


namespace marengine {

    enum class EMaterialType {
        NONE, TEX2D
    };


    class IMaterialProxy : public FRenderResource {
    public:

        virtual EMaterialType getType() const = 0;

    };


    class FMaterialProxy : public IMaterialProxy {

    };


    class FMaterialTex2D : public FMaterialProxy {
    public:

        MAR_NO_DISCARD EMaterialType getType() const final { return EMaterialType::TEX2D; }

    };


    class IMaterialStorage : public IRenderResourceStorage {
    public:

        virtual FMaterialTex2D* getTex2D(int32 index) const = 0;

        virtual size_t getCountTex2D() const = 0;

    };


    class FMaterialStorage : public IMaterialStorage {

    };


    class IMaterialFactory : public IRenderResourceFactory {
    public:

        virtual FMaterialTex2D* emplaceTex2D() = 0;

    };


    class FMaterialFactory : public IMaterialFactory {

    };




}

#endif //MARENGINE_MATERIAL_H
