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
// CRenderable
#include "../../graphics/Mesh/IMeshBatch.h"
#include "../../graphics/Mesh/Mesh.h"
// CCamera
#include "../../graphics/RenderCamera.h"
// CPointLight
#include "../../graphics/Lightning/LightDefinitions.h"
// CPythonScript
#include "../../scripting/PythonScript.h"


namespace marengine {


	struct CTag  { std::string tag{ "empty" }; };


	struct CRenderable {
	    struct MeshInfo {
	        /// @brief type used to retrieve correct implementation of FMeshProxy from FMeshStorage
            EMeshType type{ EMeshType::NONE };
            /// @brief index used to retrieve assigned to Entity FMeshProxy from FMeshStorage
	        int8 index{ -1 };
	    };
	    struct MaterialInfo {
            /// @brief type used to retrieve correct implementation of FMaterialProxy from FMaterialStorage
            //EMaterialType type{ EMaterialType::NONE };
            /// @brief index used to retrieve assigned to Entity FMaterialProxy from FMaterialStorage
            int8 index{ -1 };
	    };
	    struct BatchInfo {
	        /// @brief type used to retrieve correct implementation of FMeshBatch from FMeshBatchStorage
            EBatchType type{ EBatchType::NONE };
            /// @brief index used to retrieve FMeshBatch, with which CRenderable is rendered.
            int8 index{ -1 };
            /// @brief transform index at batch (with this we can update only one transform at batch)
            int8 transformIndex{ -1 };
            /// @brief material index at batch (with this we can update only one material at batch)
            int8 materialIndex{ -1 };
	    };

	    /// @brief Default color, so that every entity that contains CRenderable can be rendered
		maths::vec4 color{ 0.5f, 0.5f, 0.5f, 1.f };

		BatchInfo batch;
		MeshInfo mesh;
		MaterialInfo material;

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
        std::string scriptsPath{ "empty" };
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


}


#endif // !MAR_ENGINE_DEFAULT_COMPONENTS_H
