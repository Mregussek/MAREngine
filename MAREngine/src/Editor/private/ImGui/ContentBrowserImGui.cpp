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


#include "ContentBrowserImGui.h"
#include "../../public/ServiceLocatorEditor.h"
#include "../../../Core/graphics/public/MeshManager.h"
#include "../../../Core/ecs/Entity/Components.h"


namespace marengine {


    void FContentBrowserImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pMeshManager = pServiceLocator->retrieve<FHolderPtr<FMeshManager*>>()->pInstance;

        FMeshStorage* pMeshStorage{ m_pMeshManager->getStorage() };
        m_namesSize = pMeshStorage->getCountExternal() + m_offset;
        m_namesArray.at(0) = pMeshStorage->getCube()->getName();
        m_namesArray.at(1) = pMeshStorage->getPyramid()->getName();
        m_namesArray.at(2) = pMeshStorage->getSurface()->getName();
        for(uint32 i = 3; i < m_namesSize; i++) {
            m_namesArray.at(i) = pMeshStorage->getExternal(i - m_offset)->getName();
        }
    }

    bool FContentBrowserImGui::drawMeshListBox(CRenderable& cRenderable) const {
        const ImVec2 size{ ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()) };
        bool pressedButton{ false };

        if (ImGui::BeginListBox("##MeshListbox", size)) {
            for (uint16 i = 0; i < m_namesSize; i++) {
                if (ImGui::Selectable(m_namesArray.at(i))) {
                    const FMeshProxy* pMesh =
                            m_pMeshManager->getStorage()->retrieve(m_namesArray.at(i));
                    if(pMesh) {
                        cRenderable.mesh.path = std::string(pMesh->getName());
                        cRenderable.mesh.type = pMesh->getType();
                        cRenderable.mesh.index = pMesh->getIndex();
                    }
                    pressedButton = true;
                }
            }

            ImGui::EndListBox();
        }

        return pressedButton;
    }


}
