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


#include "PythonScript.h"
#include "PythonInterpreter.h"
#include "../../ProjectManager.h"
#include "../filesystem/public/FileManager.h"
#include "../ecs/Entity/Entity.h"
#include "MAREnginePy.cpp"


namespace marengine {

    
    void PythonScript::loadScript(const std::string& scriptPath) {
        const std::string from = FPythonInterpreter::changeSlashesToDots(scriptPath);
        const std::string what = FPythonInterpreter::getModuleFromPath(scriptPath);

        if (m_initialized) { m_scriptModule.reload(); }
        else {
            m_scriptModule = py::module::import(from.c_str()); 
            m_initialized = true;
        }
    
        m_module = m_scriptModule.attr(what.c_str())();
    }
    
    void PythonScript::start(const Entity& entity) const {
        if (!m_initialized) {
            return;
        }

        const auto& transform = entity.getComponent<CTransform>();
        m_module.attr("transform") = transform;
    
        if (entity.hasComponent<CPointLight>()) {
            m_module.attr("light") = entity.getComponent<CPointLight>().pointLight;
        }
    
        if (entity.hasComponent<CCamera>()) {
            m_module.attr("camera") = entity.getComponent<CCamera>();
        }

        // TODO: fix color component on pythonscript
        //if (entity.hasComponent<CRenderable>()) {
        //    const auto& renderable = entity.getComponent<CRenderable>();
        //    m_module.attr("color") = renderable;
        //}
    
        m_module.attr("start")();
    }
    
    void PythonScript::update(const Entity& entity) const {
        if (!m_initialized)
            return;
        
        auto& transform = entity.getComponent<CTransform>();
        m_module.attr("transform") = transform;
    
        if (entity.hasComponent<CPointLight>()) {
            m_module.attr("light") = entity.getComponent<CPointLight>().pointLight;
        }
    
        if (entity.hasComponent<CCamera>()) {
            m_module.attr("camera") = entity.getComponent<CCamera>();
        }

        // TODO: fix color component on pythonscript
        //if (entity.hasComponent<CRenderable>()) {
        //    const auto& color = entity.getComponent<CRenderable>();
        //    m_module.attr("color") = color;
        //}
    
        m_module.attr("update")();
    
        transform = m_module.attr("transform").cast<CTransform>();

        if (entity.hasComponent<CPointLight>()) {
            entity.getComponent<CPointLight>().pointLight =
                    m_module.attr("light").cast<FPointLight>();
        }
    
        if (entity.hasComponent<CCamera>()) {
            entity.getComponent<CCamera>() = m_module.attr("camera").cast<CCamera>();
        }

        // TODO: fix color component on pythonscript
        //if (entity.hasComponent<CRenderable>()) {
        //    auto& color = entity.getComponent<CRenderable>();
        //    color = m_module.attr("color").cast<CRenderable>();
        //}
    }


}
