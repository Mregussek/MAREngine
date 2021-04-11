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


#ifndef MARENGINE_MARSCENEJSONDEFINITIONS_H
#define MARENGINE_MARSCENEJSONDEFINITIONS_H


namespace marengine::marscenejson {

        const char* const jX{ "x" };
        const char* const jY{ "y" };
        const char* const jZ{ "z" };
        const char* const jW{ "w" };

        const char* const jDocumentation{ "Documentation" };
        const char* const jApp{ "App" };
        const char* const jAppVersion{ "AppVersion" };
        const char* const jEngine{ "Engine" };
        const char* const jEngineVersion{ "EngineVersion" };

        const char* const jProject{ "Project" };
        const char* const jProjectAbsolutePath{ "absolutePath" };
        const char* const jProjectProjectName{ "projectName" };
        const char* const jProjectProjectPath{ "projectPath" };
        const char* const jProjectAssetsPath{ "assetsPath" };
        const char* const jProjectScenesPath{ "scenesPath" };
        const char* const jProjectSceneToLoadAtStartup{ "sceneToLoadAtStartup" };
        const char* const jProjectWindowName{ "windowName" };

        const char* const jScene{ "Scene" };
        const char* const jSceneName{ "Name" };
        const char* const jSceneBackground{ "Background" };

        const char* const jEntity{ "Entity" } ;

        const char* const jCTag{ "CTag" };
        const char* const jCTagTag{ "tag" };

        const char* const jCTransform{ "CTransform" };
        const char* const jCTransformPosition{ "position" };
        const char* const jCTransformRotation{ "rotation" };
        const char* const jCTransformScale{ "scale" };

        const char* const jCRenderable{ "CRenderable" };
        const char* const jCRenderablePath{ "path" };
        const char* const jCRenderableMeshType{ "meshType" };
        const char* const jCRenderableColor{ "color" };

        const char* const jCPointLight{ "CPointLight" };
        const char* const jCPointLightPosition{ "position" };
        const char* const jCPointLightAmbient{ "ambient" };
        const char* const jCPointLightDiffuse{ "diffuse" };
        const char* const jCPointLightSpecular{ "specular" };
        const char* const jCPointLightConstant{ "constant" };
        const char* const jCPointLightLinear{ "linear" };
        const char* const jCPointLightQuadratic{ "quadratic" };
        const char* const jCPointLightShininess{ "shininess" };

        const char* const jCCamera{ "CCamera" };
        const char* const jCCameraId{ "id" };
        const char* const jCCameraPerspective{ "Perspective" };
        const char* const jCCameraPFov{ "p_fov" };
        const char* const jCCameraPAspectRatio{ "p_aspectRatio" };
        const char* const jCCameraPNear{ "p_near" };
        const char* const jCCameraPFar{ "p_far" };
        const char* const jCCameraOLeft{ "o_left" };
        const char* const jCCameraORight{ "o_right" };
        const char* const jCCameraOTop{ "o_top" };
        const char* const jCCameraOBottom{ "o_bottom" };
        const char* const jCCameraONear{ "o_near" };
        const char* const jCCameraOFar{ "o_far" };

        const char* const jCPythonScript{ "CPythonScript" };
        const char* const jCPythonScriptPath{ "path" };

}


#endif //MARENGINE_MARSCENEJSONDEFINITIONS_H
