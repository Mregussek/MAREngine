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


#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_H


#include "IRender.h"


namespace marengine {

    class Entity;
    struct CRenderable;
    struct CTransform;
    class FMeshStorage;
    class FMeshBatchStaticColor;
    class FMeshBatchStaticTex2D;


    enum class EBatchType {
        NONE, MESH_STATIC_COLOR, MESH_STATIC_TEX2D
    };


    class IMeshBatch : public FRenderResource {
    public:

        virtual void reset() = 0;

        virtual const FVertexArray& getVertices() const = 0;
        virtual const FIndicesArray& getIndices() const = 0;
        virtual const FTransformsArray& getTransforms() const = 0;

        virtual bool shouldBeBatched(const Entity& entity) const = 0;
        virtual bool canBeBatched(const Entity& entity) const = 0;
        virtual void submitToBatch(const Entity& entity) = 0;

        virtual void passVBO(int8 index) = 0;
        virtual void passIBO(int8 index) = 0;
        virtual void passTransformSSBO(int8 index) = 0;

        virtual int8 getVBO() const = 0;
        virtual int8 getIBO() const = 0;
        virtual int8 getTransformSSBO() const = 0;

        virtual void passStorage(FMeshStorage* pMeshStorage) = 0;

        virtual EBatchType getType() const = 0;

    };


    class FMeshBatch : public IMeshBatch {
    public:

        void reset() override;

        MAR_NO_DISCARD const FVertexArray& getVertices() const final { return p_vertices; }
        MAR_NO_DISCARD const FIndicesArray& getIndices() const final { return p_indices; }
        MAR_NO_DISCARD const FTransformsArray& getTransforms() const final { return p_transforms; }

        void passVBO(int8 index) final { p_vbo = index; }
        void passIBO(int8 index) final { p_ibo = index; }
        void passTransformSSBO(int8 index) final { p_transformSSBO = index; }

        MAR_NO_DISCARD int8 getVBO() const final { return p_vbo; }
        MAR_NO_DISCARD int8 getIBO() const final { return p_ibo; }
        MAR_NO_DISCARD int8 getTransformSSBO() const final { return p_transformSSBO; }

        void passStorage(FMeshStorage* pMeshStorage) final { p_pMeshStorage = pMeshStorage; }

    protected:

        FVertexArray p_vertices;
        FIndicesArray p_indices;
        FTransformsArray p_transforms;

        FMeshStorage* p_pMeshStorage{ nullptr };

        int8 p_vbo{ -1 };
        int8 p_ibo{ -1 };
        int8 p_transformSSBO{ -1 };

    };


    class FMeshBatchStatic : public FMeshBatch {

        friend class FEventsMeshBatchStatic;

    public:

        void reset() override;

        MAR_NO_DISCARD bool shouldBeBatched(const Entity& entity) const override;
        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;
        void submitToBatch(const Entity& entity) override;

    protected:

        void submitRenderable(const CRenderable& renderableComponent);
        void submitVertices(const FVertexArray& vertices);
        void submitIndices(const FIndicesArray& indices);
        void submitTransform(const CTransform& transformComponent);


        float p_shapeID{ 0.f };
        uint32_t p_indicesMaxValue{ 0 };

    };


    class FMeshBatchStaticColor : public FMeshBatchStatic {

        friend class FEventsMeshBatchStatic;

    public:

        void reset() override;

        MAR_NO_DISCARD bool shouldBeBatched(const Entity& entity) const override;
        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;
        void submitToBatch(const Entity& entity) override;

        MAR_NO_DISCARD const FColorsArray& getColors() const;

        MAR_NO_DISCARD int8 getColorSSBO() const;
        void setColorSSBO(int8 id);

        MAR_NO_DISCARD EBatchType getType() const final { return EBatchType::MESH_STATIC_COLOR; }

    private:

        void submitColor(const maths::vec4& color);


        FColorsArray m_colors;
        int8 m_colorsSSBO{ -1 };

    };


    class FMeshBatchStaticTex2D : public FMeshBatchStatic {
    public:

        MAR_NO_DISCARD EBatchType getType() const final { return EBatchType::MESH_STATIC_TEX2D; }

    };


    class IMeshBatchStorage : public IRenderResourceStorage {
    public:

        virtual FMeshBatchStaticColor* getStaticColor(int8 index) const = 0;
        virtual FMeshBatchStaticTex2D* getStaticTex2D(int8 index) const = 0;

        virtual size_t getCountStaticColor() const = 0;
        virtual size_t getCountStaticTex2D() const = 0;

    };


    class FMeshBatchStorage : public IMeshBatchStorage {

        friend class FMeshBatchFactory;


    public:

        MAR_NO_DISCARD FMeshBatchStaticColor* getStaticColor(int8 index) const final;
        MAR_NO_DISCARD FMeshBatchStaticTex2D* getStaticTex2D(int8 index) const final;

        MAR_NO_DISCARD size_t getCountStaticColor() const final;
        MAR_NO_DISCARD size_t getCountStaticTex2D() const final;

        void reset() final;

    private:

        std::vector<FMeshBatchStaticColor> m_staticColors;
        std::vector<FMeshBatchStaticTex2D> m_staticTex2D;

    public:

        MAR_NO_DISCARD auto getArrayStaticColor() ->decltype(&m_staticColors) const;
        MAR_NO_DISCARD auto getArrayStaticTex2D() ->decltype(&m_staticTex2D) const;


    };


    class IMeshBatchFactory : public IRenderResourceFactory {
    public:

        virtual FMeshBatchStaticColor* emplaceStaticColor() = 0;
        virtual FMeshBatchStaticTex2D* emplaceStaticTex2D() = 0;

    };


    class FMeshBatchFactory : public IMeshBatchFactory {
    public:

        MAR_NO_DISCARD FMeshBatchStaticColor* emplaceStaticColor() final;
        MAR_NO_DISCARD FMeshBatchStaticTex2D* emplaceStaticTex2D() final;

        MAR_NO_DISCARD FMeshBatchStorage* getStorage() const;

        void passMeshStorage(FMeshStorage* pMeshStorage) { m_pMeshStorage = pMeshStorage; }

    private:

        FMeshBatchStorage m_storage;
        FMeshStorage* m_pMeshStorage{ nullptr };

    };


}


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_H
