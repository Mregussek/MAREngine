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


#ifndef MARENGINE_IGRAPHICSPIPELINE_H
#define MARENGINE_IGRAPHICSPIPELINE_H


#include "../../../mar.h"


namespace marengine {

    class FWindow;


    enum class EGraphicsPipelineType {
        NONE, COLOR, TEXTURE2D
    };


    template<typename TDerivedPipeline>
    class IGraphicsPipeline {
    public:

        template<typename TVertexShader>
        void passVertexShader(TVertexShader* pVertexShader) {
            static_cast<TDerivedPipeline*>(this)->passVertexShader(pVertexShader);
        }

        template<typename TFragmentShader>
        void passFragmentShader(TFragmentShader* pFragmentShader) {
            static_cast<TDerivedPipeline*>(this)->passFragmentShader(pFragmentShader);
        }

        template<typename TVertexBuffer>
        void passVertexBuffer(TVertexBuffer* pVertexBuffer) {
            static_cast<TDerivedPipeline*>(this)->passVertexBuffer(pVertexBuffer);
        }

        template<typename TIndexBuffer>
        void passIndexBuffer(TIndexBuffer* pIndexBuffer) {
            static_cast<TDerivedPipeline*>(this)->passIndexBuffer(pIndexBuffer);
        }

        template<typename TCameraBuffer>
        void passCameraBuffer(TCameraBuffer* pCameraBuffer) {
            static_cast<TDerivedPipeline*>(this)->passCameraBuffer(pCameraBuffer);
        }

        void create() {
            static_cast<TDerivedPipeline*>(this)->create();
        }

        void close() {
            static_cast<TDerivedPipeline*>(this)->close();
        }

    };


    template<typename TMeshTypePipeline>
    class FGraphicsPipelineMesh :
            public IGraphicsPipeline<FGraphicsPipelineMesh<TMeshTypePipeline>> {
    public:

        // What FGraphicsPipelineMesh brings to Derived classes...

        template<typename TTransformBuffer>
        void passTransformBuffer(TTransformBuffer* pTransformBuffer) {
            static_cast<TMeshTypePipeline*>(this)->passTransformBuffer(pTransformBuffer);
        }

        // Copy paste ...

        template<typename TVertexShader>
        void passVertexShader(TVertexShader* pVertexShader) {
            static_cast<TMeshTypePipeline*>(this)->passVertexShader(pVertexShader);
        }

        template<typename TFragmentShader>
        void passFragmentShader(TFragmentShader* pFragmentShader) {
            static_cast<TMeshTypePipeline*>(this)->passFragmentShader(pFragmentShader);
        }

        template<typename TVertexBuffer>
        void passVertexBuffer(TVertexBuffer* pVertexBuffer) {
            static_cast<TMeshTypePipeline*>(this)->passVertexBuffer(pVertexBuffer);
        }

        template<typename TIndexBuffer>
        void passIndexBuffer(TIndexBuffer* pIndexBuffer) {
            static_cast<TMeshTypePipeline*>(this)->passIndexBuffer(pIndexBuffer);
        }

        template<typename TCameraBuffer>
        void passCameraBuffer(TCameraBuffer* pCameraBuffer) {
            static_cast<TMeshTypePipeline*>(this)->passCameraBuffer(pCameraBuffer);
        }

        void create() {
            static_cast<TMeshTypePipeline*>(this)->create();
        }

        void close() {
            static_cast<TMeshTypePipeline*>(this)->close();
        }

    };


    template<typename TLibraryDependentPipeline>
    class FGraphicsPipelineColorMesh :
            public FGraphicsPipelineMesh<FGraphicsPipelineColorMesh<TLibraryDependentPipeline>> {
    public:

        // What FGraphicsPipelineColorMesh brings to Derived classes...

        template<typename TColorBuffer>
        void passColorBuffer(TColorBuffer* pColorBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passColorBuffer(pColorBuffer);
        }

        // Copy paste ...

        template<typename TTransformBuffer>
        void passTransformBuffer(TTransformBuffer* pTransformBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passTransformBuffer(pTransformBuffer);
        }

        template<typename TVertexShader>
        void passVertexShader(TVertexShader* pVertexShader) {
            static_cast<TLibraryDependentPipeline*>(this)->passVertexShader(pVertexShader);
        }

        template<typename TFragmentShader>
        void passFragmentShader(TFragmentShader* pFragmentShader) {
            static_cast<TLibraryDependentPipeline*>(this)->passFragmentShader(pFragmentShader);
        }

        template<typename TVertexBuffer>
        void passVertexBuffer(TVertexBuffer* pVertexBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passVertexBuffer(pVertexBuffer);
        }

        template<typename TIndexBuffer>
        void passIndexBuffer(TIndexBuffer* pIndexBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passIndexBuffer(pIndexBuffer);
        }

        template<typename TCameraBuffer>
        void passCameraBuffer(TCameraBuffer* pCameraBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passCameraBuffer(pCameraBuffer);
        }

        void create() {
            static_cast<TLibraryDependentPipeline*>(this)->create();
        }

        void close() {
            static_cast<TLibraryDependentPipeline*>(this)->close();
        }

    };


    template<typename TLibraryDependentPipeline>
    class FGraphicsPipelineTexture2DMesh :
            public FGraphicsPipelineMesh<FGraphicsPipelineTexture2DMesh<TLibraryDependentPipeline>> {
    public:

        // What FGraphicsPipelineTexture2DMesh brings to Derived classes...

        template<typename TTextureSamples>
        void passTextureSamples(TTextureSamples* pTextureSamples) {
            static_cast<TLibraryDependentPipeline*>(this)->passTextureSamples(pTextureSamples);
        }

        // Copy paste ...

        template<typename TTransformBuffer>
        void passTransformBuffer(TTransformBuffer* pTransformBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passTransformBuffer(pTransformBuffer);
        }

        template<typename TVertexShader>
        void passVertexShader(TVertexShader* pVertexShader) {
            static_cast<TLibraryDependentPipeline*>(this)->passVertexShader(pVertexShader);
        }

        template<typename TFragmentShader>
        void passFragmentShader(TFragmentShader* pFragmentShader) {
            static_cast<TLibraryDependentPipeline*>(this)->passFragmentShader(pFragmentShader);
        }

        template<typename TVertexBuffer>
        void passVertexBuffer(TVertexBuffer* pVertexBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passVertexBuffer(pVertexBuffer);
        }

        template<typename TIndexBuffer>
        void passIndexBuffer(TIndexBuffer* pIndexBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passIndexBuffer(pIndexBuffer);
        }

        template<typename TCameraBuffer>
        void passCameraBuffer(TCameraBuffer* pCameraBuffer) {
            static_cast<TLibraryDependentPipeline*>(this)->passCameraBuffer(pCameraBuffer);
        }

        void create() {
            static_cast<TLibraryDependentPipeline*>(this)->create();
        }

        void close() {
            static_cast<TLibraryDependentPipeline*>(this)->close();
        }

    };


}


#endif //MARENGINE_IGRAPHICSPIPELINE_H
