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


#ifndef MARENGINE_MATERIALOPENGL_H
#define MARENGINE_MATERIALOPENGL_H


#include "../../public/Material.h"


namespace marengine {

    struct CRenderable;


    class FMaterialTex2DOpenGL : public FMaterialTex2D {
    public:

        void destroy() final;

        void bind() const final;
        void load() final;

    private:

        uint32 m_id{ 0 };

    };


    class FMaterialStorageOpenGL : public FMaterialStorage {

        friend class FMaterialFactoryOpenGL;

    public:

        MAR_NO_DISCARD FMaterialTex2D* getTex2D(int32 index) const final;
        MAR_NO_DISCARD uint32 getCountTex2D() const final;

        MAR_NO_DISCARD FMaterial* retrieve(const CRenderable& cRenderable) const final;

        MAR_NO_DISCARD bool isAlreadyLoadedTex2D(const std::string& texture) const final;

        void reset() final;

    private:

        std::vector<FMaterialTex2DOpenGL> m_textures2D;

    };


    class FMaterialFactoryOpenGL : public FMaterialFactory {

        friend class FRenderContextOpenGL;

    public:

        MAR_NO_DISCARD FMaterialTex2D* emplaceTex2D() final;
        MAR_NO_DISCARD FMaterialStorage* getStorage() const final;

    private:

        FMaterialStorageOpenGL m_storage;

    };


}


#endif //MARENGINE_MATERIALOPENGL_H
