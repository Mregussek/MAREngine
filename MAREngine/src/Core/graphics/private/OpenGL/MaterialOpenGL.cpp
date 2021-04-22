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


#include "MaterialOpenGL.h"
#include "../../../../Logging/Logger.h"


namespace marengine {

    struct FTex2DFormats {
        GLenum internal{ GL_NONE };
        GLenum data{ GL_NONE };
    };

    static FTex2DFormats getFormats(int32 bitPerPixel) {
        FTex2DFormats formats;
        if (bitPerPixel == 4) {
            formats.internal = GL_RGBA8;
            formats.data = GL_RGBA;
        }
        else if (bitPerPixel == 3) {
            formats.internal = GL_RGB8;
            formats.data = GL_RGB;
        }
        return formats;
    }

    static void loadTexture2D(uint32& id, const char* path) {
        int32 width, height, bitPerPixel;
        unsigned char* localBuffer;

        stbi_set_flip_vertically_on_load(true);
        localBuffer = stbi_load(path, &width, &height, &bitPerPixel, 0);

        if (localBuffer) {
            const FTex2DFormats formats{ getFormats(bitPerPixel) };

            GL_FUNC( glCreateTextures(GL_TEXTURE_2D, 1, &id) );
            GL_FUNC( glTextureStorage2D(id, 1, formats.internal, width, height) );
            GL_FUNC( glTextureSubImage2D(id, 0, 0, 0, width, height,
                                         formats.data, GL_UNSIGNED_BYTE, localBuffer) );

            GL_FUNC ( glGenerateMipmap(GL_TEXTURE_2D) );

            GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );

            GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
            GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

            stbi_image_free(localBuffer);
        }
    }


    void FMaterialTex2DOpenGL::destroy() {
        GL_FUNC( glDeleteTextures(1, &m_id) );
    }

    void FMaterialTex2DOpenGL::bind() const {
        GL_FUNC( glActiveTexture(GL_TEXTURE0 + p_info.sampler) );
        GL_FUNC( glBindTexture(GL_TEXTURE_2D, m_id) );
    }

    void FMaterialTex2DOpenGL::load() {
        loadTexture2D(m_id, p_info.path.c_str());
    }


    FMaterialTex2D* FMaterialStorageOpenGL::getTex2D(int32 index) const {
        return (FMaterialTex2D*)&m_textures2D.at(index);
    }

    uint32 FMaterialStorageOpenGL::getCountTex2D() const {
        return m_textures2D.size();
    }

    FMaterial* FMaterialStorageOpenGL::retrieve(const CRenderable& cRenderable) const {
        // TODO: implement FMaterialStorageOpenGL::retrieve
        return nullptr;
    }

    bool FMaterialStorageOpenGL::isAlreadyLoadedTex2D(const std::string& texture) const {
        // TODO: implement FMaterialStorageOpenGL::isAlreadyLoadedTex2D
        return false;
    }

    void FMaterialStorageOpenGL::reset() {
        for(auto& texture : m_textures2D) {
            texture.destroy();
        }
        m_textures2D.clear();
    }


    template<typename TReturnType, typename TArray>
    static TReturnType* emplaceBufferAtArray(TArray& array) {
        auto& variable{ array.emplace_back() };
        const auto currentSize{ (int32)array.size() };
        variable.setIndex(currentSize - 1);
        return (TReturnType*)&variable;
    }

    FMaterialTex2D* FMaterialFactoryOpenGL::emplaceTex2D() {
        return emplaceBufferAtArray<FMaterialTex2D>(m_storage.m_textures2D);
    }


}
