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


#ifndef MARENGINE_SERVICEMANAGEREDITOR_H
#define MARENGINE_SERVICEMANAGEREDITOR_H


#include "../../mar.h"
#include "IEditorWidget.h"
#include "../../Core/IServiceManager.h"


namespace marengine {

    class FServiceLocatorEditor;


    class FServiceManagerEditor :
            public IServiceManager<FServiceManagerEditor, FEditorWidget> {
    public:

        template<EEditorContextType TEditorType>
        void create(FServiceLocatorEditor* pServiceLocator);

        void emplace(FEditorWidget* service);
        void pop(FEditorWidget* service);

        void onCreate() const;
        void onUpdate() const;
        void onDestroy() const;

    private:

        std::array<FEditorWidget*, 9> m_services;
        size_t m_insertValue{ 0 };

    };


}


#include "../private/ServiceManagerEditor.inl"


#endif //MARENGINE_SERVICEMANAGEREDITOR_H