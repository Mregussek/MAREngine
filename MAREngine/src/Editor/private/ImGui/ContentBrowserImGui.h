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


#ifndef MARENGINE_CONTENTBROWSERIMGUI_H
#define MARENGINE_CONTENTBROWSERIMGUI_H


#include "../../../mar.h"
#include "../../public/EditorWidget.h"


namespace marengine {

    class FMeshManager;
    struct CRenderable;


    class FContentBrowserImGui : public FContentBrowserEditorWidget {
    public:

        void create(FServiceLocatorEditor* pServiceLocator) final;

        MAR_NO_DISCARD bool drawMeshListBox(CRenderable& cRenderable) const final;

    private:

        static constexpr uint16 m_offset{ 3 }; // + 3 because Cube, Pyramid Surface
        std::array<const char*, 100> m_namesArray;
        uint16 m_namesSize{ 0 };
        FMeshManager* m_pMeshManager{ nullptr };

    };


}



#endif //MARENGINE_CONTENTBROWSERIMGUI_H
