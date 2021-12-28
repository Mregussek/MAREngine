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


#include "IMeshBatch.h"


namespace marengine {

    struct CRenderable;
    struct CTransform;
    class FMaterialStorage;
    class FMaterialTex2D;


    class FMeshBatch : public IMeshBatch {
    public:

        void reset() override;

        MAR_NO_DISCARD const FVertexArray& getVertices() const final;
        MAR_NO_DISCARD const FIndicesArray& getIndices() const final;
        MAR_NO_DISCARD const FTransformsArray& getTransforms() const final;

        void updateVertices(const Entity& entity) final;
        void updateIndices(const Entity& entity) final;
        void updateTransform(const Entity& entity) final;

        void passVBO(int32 index) final ;
        void passIBO(int32 index) final;
        void passTransformSSBO(int32 index) final;

        MAR_NO_DISCARD int32 getVBO() const final;
        MAR_NO_DISCARD int32 getIBO() const final;
        MAR_NO_DISCARD int32 getTransformSSBO() const final;

        void passMeshStorage(FMeshStorage* pMeshStorage) final;
        void passMaterialStorage(FMaterialStorage* pMaterialStorage) final;

    protected:

        FVertexArray p_vertices;
        FIndicesArray p_indices;
        FTransformsArray p_transforms;

        FMeshStorage* p_pMeshStorage{ nullptr };
        FMaterialStorage* p_pMaterialStorage{ nullptr };

        int32 p_vbo{ -1 };
        int32 p_ibo{ -1 };
        int32 p_transformSSBO{ -1 };

    };


    class FMeshBatchStatic : public FMeshBatch {
    public:

        void reset() override;

        MAR_NO_DISCARD bool shouldBeBatched(const Entity& entity) const override;
        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;
        void submitToBatch(const Entity& entity) override;

    protected:

        void submitRenderable(CRenderable& cRenderable);
        void submitVertices(CRenderable& cRenderable, const FVertexArray& vertices);
        void submitIndices(CRenderable& cRenderable, const FIndicesArray& indices);
        void submitTransform(const CTransform& transformComponent);


        float p_shapeID{ 0.f };
        uint32_t p_indicesMaxValue{ 0 };

    };


    class FMeshBatchStaticColor : public FMeshBatchStatic {
    public:

        void reset() override;

        MAR_NO_DISCARD bool shouldBeBatched(const Entity& entity) const override;
        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;
        void submitToBatch(const Entity& entity) override;

        MAR_NO_DISCARD const FColorsArray& getColors() const;

        void updateColor(const Entity& entity);

        MAR_NO_DISCARD int32 getColorSSBO() const;
        void passColorSSBO(int32 id);

        MAR_NO_DISCARD EBatchType getType() const final;

    private:

        void submitColor(const maths::vec4& color);


        FColorsArray m_colors;
        int32 m_colorsSSBO{ -1 };

    };


    class FMeshBatchStaticTex2D : public FMeshBatchStatic {
    public:

        void reset() override;

        MAR_NO_DISCARD bool shouldBeBatched(const Entity& entity) const override;
        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;
        void submitToBatch(const Entity& entity) override;

        MAR_NO_DISCARD EBatchType getType() const final;
        MAR_NO_DISCARD const std::vector<int32>& getTextureIndexes() const;

    private:

        void submitTexture(FMaterialTex2D* pTexture2D);


        std::vector<int32> m_textureIndexes;

    };


    class FMeshBatchStorageStaticColor {
    public:

        MAR_NO_DISCARD FMeshBatchStaticColor* get(int32 index) const;
        MAR_NO_DISCARD uint32 getCount() const;
        MAR_NO_DISCARD bool isEmpty() const;

    private:

        std::vector<FMeshBatchStaticColor> m_meshBatches;

    public:

        MAR_NO_DISCARD auto getArray() ->decltype(&m_meshBatches) const;

    };

    class FMeshBatchStorageStaticTex2D {
    public:

        MAR_NO_DISCARD FMeshBatchStaticTex2D* get(int32 index) const;
        MAR_NO_DISCARD uint32 getCount() const;
        MAR_NO_DISCARD bool isEmpty() const;

    private:

        std::vector<FMeshBatchStaticTex2D> m_meshBatches;

    public:

        MAR_NO_DISCARD auto getArray() ->decltype(&m_meshBatches) const;

    };


    class FMeshBatchStorage : public IMeshBatchStorage {

        friend class FMeshBatchFactory;

    public:

        MAR_NO_DISCARD FMeshBatchStorageStaticColor* getStorageStaticColor() const final;
        MAR_NO_DISCARD FMeshBatchStorageStaticTex2D* getStorageStaticTex2D() const final;
        MAR_NO_DISCARD FMeshBatch* retrieve(const CRenderable& cRenderable) const;

        void reset() final;

    private:

        FMeshBatchStorageStaticColor m_storageStaticColor;
        FMeshBatchStorageStaticTex2D m_storageStaticTex2D;

    };


    class FMeshBatchFactory : public IMeshBatchFactory {
    public:

        MAR_NO_DISCARD FMeshBatchStaticColor* emplaceStaticColor() final;
        MAR_NO_DISCARD FMeshBatchStaticTex2D* emplaceStaticTex2D() final;

        template<typename TMeshBatchStorage>
        MAR_NO_DISCARD FMeshBatchStatic* emplaceStatic(TMeshBatchStorage* pMeshBatchStorage);

        MAR_NO_DISCARD FMeshBatchStorage* getStorage() const;

        void passMeshStorage(FMeshStorage* pMeshStorage);
        void passMaterialStorage(FMaterialStorage* pMaterialStorage);

    private:

        FMeshBatchStorage m_storage;
        FMeshStorage* m_pMeshStorage{ nullptr };
        FMaterialStorage* m_pMaterialStorage{ nullptr };

    };


}


#include "../private/MeshBatch.inl"


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_H
