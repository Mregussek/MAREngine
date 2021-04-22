/***********************************************************************
* @internal @copyright
*
*       MAREngine - open source 3D game engine
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


#ifndef MARENGINE_GRAPHICSOPENGL_H
#define MARENGINE_GRAPHICSOPENGL_H


#include "../../../../mar.h"
#include "../../public/IRender.h"
#include "RendererOpenGL.h"
#include "BufferOpenGL.h"
#include "PipelineOpenGL.h"
#include "ShadersOpenGL.h"
#include "FramebufferOpenGL.h"
#include "MaterialOpenGL.h"


namespace marengine {

    class FWindow;


    class FRenderContextOpenGL : public FRenderContext {
    public:

        bool create(FWindow* pWindow) final;
        void close() final;

        void prepareFrame() final;
        void endFrame() final;

        MAR_NO_DISCARD ERenderContextType getType() const final;

        MAR_NO_DISCARD FBufferStorage* getBufferStorage() const final;
        MAR_NO_DISCARD FShadersStorage* getShadersStorage() const final;
        MAR_NO_DISCARD FPipelineStorage* getPipelineStorage() const final;
        MAR_NO_DISCARD FFramebufferStorage* getFramebufferStorage() const final;
        MAR_NO_DISCARD FMaterialStorage* getMaterialStorage() const final;

        MAR_NO_DISCARD FBufferFactory* getBufferFactory() const final;
        MAR_NO_DISCARD FShadersFactory* getShadersFactory() const final;
        MAR_NO_DISCARD FPipelineFactory* getPipelineFactory() const final;
        MAR_NO_DISCARD FFramebufferFactory* getFramebufferFactory() const final;
        MAR_NO_DISCARD FMaterialFactory* getMaterialFactory() const final;

    private:

        FPipelineFactoryOpenGL m_pipelineFactory;
        FShadersFactoryOpenGL m_shadersFactory;
        FBufferFactoryOpenGL m_bufferFactory;
        FFramebufferFactoryOpenGL m_framebufferFactory;
        FMaterialFactoryOpenGL m_materialFactory;

        FWindow* m_pWindow{ nullptr };

    };


}



#endif //MARENGINE_GRAPHICSOPENGL_H
