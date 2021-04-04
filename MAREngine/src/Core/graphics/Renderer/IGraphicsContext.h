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


#ifndef MARENGINE_IGRAPHICSCONTEXT_H
#define MARENGINE_IGRAPHICSCONTEXT_H


namespace marengine {

    class FWindow;
    class FShaderPipeline;


    enum class EGraphicsContextType {
        NONE, OPENGL, VULKAN
    };

    enum class EGraphicsPipelineType {
        NONE, MESH_COLOR, MESH_TEXTURE2D
    };



    class IGraphicsContext {
    public:

        virtual bool create(FWindow* pWindow) = 0;

        virtual void close() = 0;

        virtual void prepareFrame() = 0;

        virtual void endFrame() = 0;

        virtual EGraphicsContextType getType() const = 0;

    };
    

    class FGraphicsContext : public IGraphicsContext { };

    

    class IGraphicsPipeline {
    public:

        virtual void bind() = 0;

    };
    
    
}


#endif //MARENGINE_IGRAPHICSCONTEXT_H
