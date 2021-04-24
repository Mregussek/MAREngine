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


#ifndef MAR_ENGINE_DEFAULT_COMPONENTS_H
#define MAR_ENGINE_DEFAULT_COMPONENTS_H


#include "../../../mar.h"
#include "../../graphics/public/MeshBatch.h"   // CRenderable
#include "../../graphics/public/Mesh.h"    // CRenderable
#include "../../graphics/public/RenderCamera.h"    // CCamera
#include "../../graphics/public/Light.h"  // CPointLight
#include "../../graphics/public/LightBatch.h"  // CPointLight
#include "../../graphics/public/Material.h"  // CRenderable
#include "../../scripting/PythonScript.h"   // CPythonScript


namespace marengine {


	struct CTag  { std::string tag{ "New Entity" }; };


	struct CRenderable {
	    struct MeshInfo {
	        /// @brief path used to retrieve external FMeshProxy from FMeshStorage (can be empty if used default Mesh)
	        std::string path{};
	        /// @brief type used to retrieve correct implementation of FMeshProxy from FMeshStorage
            EMeshType type{ EMeshType::NONE };
            /// @brief index used to retrieve assigned to Entity FMeshProxy from FMeshStorage (can be -1 if default Mesh used)
            int32 index{ -1 };
	    };
	    struct MaterialInfo {
            /// @brief type used to retrieve correct implementation of FMaterialProxy from FMaterialStorage
            EMaterialType type{ EMaterialType::NONE };
            /// @brief index used to retrieve assigned to Entity FMaterialProxy from FMaterialStorage
            int32 index{ -1 };

            bool isValid() const { return index != -1 && type != EMaterialType::NONE; }
	    };
	    struct BatchInfo {
	        /// @brief type used to retrieve correct implementation of FMeshBatch from FMeshBatchStorage
            EBatchType type{ EBatchType::NONE };
            /// @brief index used to retrieve FMeshBatch, with which CRenderable is rendered.
            int32 index{ -1 };
            /// @brief transform index at batch (with this we can update only one transform at batch)
            int32 transformIndex{ -1 };
            /// @brief material index at batch (with this we can update only one material at batch)
            int32 materialIndex{ -1 };

            int32 startVert{ -1 };
            int32 endVert{ -1 };

            int32 startInd{ -1 };
            int32 endInd{ -1 };
	    };

	    /// @brief Default color, so that every entity that contains CRenderable can be rendered
		maths::vec4 color{ 0.5f, 0.5f, 0.5f, 1.f };

        MeshInfo mesh;
		BatchInfo batch;
		MaterialInfo material;

		bool isEntityRendered() const {
            return batch.type != EBatchType::NONE && batch.index != -1;
		}

		bool isBatchUpdateValid() const {
		    return batch.startVert != -1
		        && batch.endVert != -1
		        && batch.startInd != -1
		        && batch.endInd != -1;
		}

	};


	struct CTransform {

		/**
		 * @brief Calculates transform matrix from position, rotation and scale vec3's and returns it.
		 * @return calculated transform matrix
		 */
		MAR_NO_DISCARD maths::mat4 getTransform() const;

		maths::vec3 position{ 0.f, 0.f, 0.f };
		maths::vec3 rotation{ 0.f, 0.f, 0.f };
		maths::vec3 scale{ 1.f, 1.f, 1.f };

	};


    struct CPointLight {
        struct BatchInfo {
            ELightBatchType type{ ELightBatchType::NONE };
            int8 index{ -1 };
        };

        FPointLight pointLight;
        BatchInfo batch;

    };


    struct CPythonScript {
        PythonScript pythonScript;
        std::string scriptsPath{};
    };


    struct CCamera {

        /**
         * @brief Method checks, if renderCamera is a main one for current scene and returns result.
         * @return returns true, if renderCamera instance is main camera for current scene.
         */
        MAR_NO_DISCARD bool isMainCamera() const;

        RenderCamera renderCamera;
        std::string id{ "secondary" };   // must contain "main" to be main camera

        float p_fov{ 45.f };
        float p_aspectRatio{ 4.f / 3.f };
        float p_near{ 0.01f };
        float p_far{ 100.0f };

        float o_left{ -10.f };
        float o_right{ 10.f };
        float o_top{ 10.f };
        float o_bottom{ -10.f };
        float o_near{ 0.01f };
        float o_far{ 100.0f };

        bool Perspective{ true }; // true - perspective / false - orthographic

    };


    struct CPlayModeStorage {

        typedef std::variant<CTransform,
                CRenderable,
                CPointLight> ComponentVariant;

        enum class ComponentType {
            NONE, TRANSFORM, RENDERABLE, POINTLIGHT
        };

        std::unordered_map<ComponentType, ComponentVariant> components;

    };


    enum class EEventType {
        NONE,
        RENDERABLE_COLOR_UPDATE,
        RENDERABLE_MESH_UPDATE,
        RENDERABLE_TEX2D_LOAD,
        PYTHONSCRIPT_OPEN,
        PYTHONSCRIPT_CREATE_ASSIGN,
        PYTHONSCRIPT_ASSIGN
    };

    struct FFilesystemDialogInfo;

    struct CEvent {

        const FFilesystemDialogInfo* pFilesystemDialogInfo{ nullptr };
        EEventType eventUpdateType{ EEventType::NONE };

        CEvent(EEventType eUpdateType) :
                eventUpdateType(eUpdateType)
        {}

        CEvent(EEventType eUpdateType, const FFilesystemDialogInfo* pFsDialogInfo) :
                eventUpdateType(eUpdateType),
                pFilesystemDialogInfo(pFsDialogInfo)
        {}

    };


}


#endif // !MAR_ENGINE_DEFAULT_COMPONENTS_H
