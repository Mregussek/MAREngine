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


#include "EditorWidgetsServiceManager.h"


namespace marengine {


    void FEditorWidgetsServiceManager::emplace(FEditorWidget* service) {
        m_services.at(m_insertValue) = service;
        m_insertValue++;
    }

    void FEditorWidgetsServiceManager::pop(FEditorWidget* service) {
        auto serviceExists = [service](FEditorWidget* iterService){
            return iterService == service;
        };
        auto newEnd = std::remove_if(m_services.begin(), m_services.begin() + m_insertValue, serviceExists);
        m_insertValue = std::distance(m_services.begin(), newEnd);
    }

    void FEditorWidgetsServiceManager::onCreate() const {
        std::for_each(m_services.begin(), m_services.begin() + m_insertValue, [](FEditorWidget* service) {
            service->onCreation();
        });
    }

    void FEditorWidgetsServiceManager::onUpdate() const {
        std::for_each(m_services.begin(), m_services.begin() + m_insertValue, [](FEditorWidget* service) {
            service->beginFrame();
            service->onBeginFrame();
        });

        std::for_each(m_services.begin(), m_services.begin() + m_insertValue, [](FEditorWidget* service) {
            service->updateFrame();
            service->onUpdateFrame();
        });

        std::for_each(m_services.begin(), m_services.begin() + m_insertValue, [](FEditorWidget* service) {
            service->endFrame();
            service->onEndFrame();
        });
    }

    void FEditorWidgetsServiceManager::onDestroy() const {
        std::for_each(m_services.begin(), m_services.begin() + m_insertValue, [](FEditorWidget* service) {
            service->destroy();
            service->onDestruction();
            service = nullptr; // as every type should passed allocated on stack, we can set nullptr
        });
    }


}
