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


#ifndef MARENGINE_FEVENTSCOMPONENTEDITOR_INL
#define MARENGINE_FEVENTSCOMPONENTEDITOR_INL


#include "../public/EventsComponentEditor.h"
#include "../../Core/ecs/Entity/Entity.h"


namespace marengine {


    template<typename TComponent>
    void FEventsComponentEditor::onAdd(const Entity& entity) {

    }

    template<typename TComponent>
    void FEventsComponentEditor::onUpdate(const Entity& entity) {

    }

    template<typename TComponent>
    void FEventsComponentEditor::onRemove(const Entity& entity) {

    }


    template<>
    void FEventsComponentEditor::onUpdate<CTransform>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onAdd<CRenderable>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onUpdate<CRenderable>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onRemove<CRenderable>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onAdd<CPointLight>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onUpdate<CPointLight>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onRemove<CPointLight>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onAdd<CCamera>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onUpdate<CCamera>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onRemove<CCamera>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onAdd<CPythonScript>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onUpdate<CPythonScript>(const Entity& entity);

    template<>
    void FEventsComponentEditor::onRemove<CPythonScript>(const Entity& entity);


}


#endif //MARENGINE_FEVENTSCOMPONENTEDITOR_INL
