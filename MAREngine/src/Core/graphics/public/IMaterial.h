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


#ifndef MARENGINE_IMATERIAL_H
#define MARENGINE_IMATERIAL_H


#include "IRenderResource.h"


namespace marengine {

    class FMaterialProxy;
    class FMaterialTex2D;
    struct CRenderable;


    enum class EMaterialType {
        NONE, TEX2D
    };

    struct FTex2DInfo {
        std::string path{};
        uint32 sampler{ 0 };
        int32 id{ -1 };
    };


    class IMaterialProxy : public FRenderResource {
    public:

        virtual void destroy() = 0;

        virtual void bind() const = 0;
        virtual void load() = 0;
        virtual EMaterialType getType() const = 0;

    };


    class IMaterialStorage : public IRenderResourceStorage {
    public:

        virtual FMaterialTex2D* getTex2D(int32 index) const = 0;
        virtual uint32 getCountTex2D() const = 0;

        virtual const FMaterialProxy* retrieve(const CRenderable& cRenderable) const = 0;

        virtual const FMaterialProxy* isAlreadyLoadedTex2D(const std::string& texture) const = 0;

    };


    class IMaterialFactory : public FRenderResourceFactory {
    public:

        virtual FMaterialTex2D* emplaceTex2D(const std::string& path) = 0;

        virtual FMaterialStorage* getStorage() const = 0;

    };


}


#endif //MARENGINE_IMATERIAL_H
