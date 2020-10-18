/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "PythonScript.h"
#include "MAREnginePy.cpp"
#include "ScriptingLogs.h"
#include "../../Engine.h"
#include "../ecs/Entity/Entity.h"
#include "../ecs/Components/Components.h"


namespace mar::scripting {

    
    void PythonScript::loadScript(std::string path_to_script) {
        const std::string from = changeSlashesToDots(path_to_script);
        const std::string what = getModuleFromPath(path_to_script);
    
        if (m_initialized) { m_scriptModule.reload(); }
        else { 
            m_scriptModule = py::module::import(from.c_str()); 
            m_initialized = true;
        }
    
        m_module = m_scriptModule.attr(what.c_str())();
    
        SCRIPTING_INFO("PYTHON_SCRIPT: Loaded script {} from {}", what, from);
    }
    
    void PythonScript::start(const ecs::Entity& entity) const {
        if (!m_initialized)
            return;
    
        const auto& transform = entity.getComponent<ecs::TransformComponent>();
        m_module.attr("transform") = transform;
    
        if (entity.hasComponent<ecs::LightComponent>()) {
            const auto& light = entity.getComponent<ecs::LightComponent>();
            m_module.attr("light") = light;
        }
    
        if (entity.hasComponent<ecs::CameraComponent>()) {
            const auto& camera = entity.getComponent<ecs::CameraComponent>();
            m_module.attr("camera") = camera;
        }
    
        if (entity.hasComponent<ecs::ColorComponent>()) {
            const auto& color = entity.getComponent<ecs::ColorComponent>();
            m_module.attr("color") = color;
        }
    
        m_module.attr("start")();
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: Calling start method at python script");
    }
    
    void PythonScript::update(const ecs::Entity& entity) const {
        if (!m_initialized)
            return;
        
        auto& transform = entity.getComponent<ecs::TransformComponent>();
        m_module.attr("transform") = transform;
    
        if (entity.hasComponent<ecs::LightComponent>()) {
            const auto& light = entity.getComponent<ecs::LightComponent>();
            m_module.attr("light") = light;
        }
    
        if (entity.hasComponent<ecs::CameraComponent>()) {
            const auto& camera = entity.getComponent<ecs::CameraComponent>();
            m_module.attr("camera") = camera;
        }
    
        if (entity.hasComponent<ecs::ColorComponent>()) {
            const auto& color = entity.getComponent<ecs::ColorComponent>();
            m_module.attr("color") = color;
        }
    
        m_module.attr("update")();
    
        transform = m_module.attr("transform").cast<ecs::TransformComponent>();
        transform.recalculate();
    
        if (entity.hasComponent<ecs::LightComponent>()) {
            auto& light = entity.getComponent<ecs::LightComponent>();
            light = m_module.attr("light").cast<ecs::LightComponent>();
        }
    
        if (entity.hasComponent<ecs::CameraComponent>()) {
            auto& camera = entity.getComponent<ecs::CameraComponent>();
            camera = m_module.attr("camera").cast<ecs::CameraComponent>();
        }
    
        if (entity.hasComponent<ecs::ColorComponent>()) {
            auto& color = entity.getComponent<ecs::ColorComponent>();
            color = m_module.attr("color").cast<ecs::ColorComponent>();
        }
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: Calling update method at python script");
    }
    
    void PythonScript::appendCurrentPath() {
        static pybind11::scoped_interpreter guard{};
    
        auto os = py::module::import("os");
        auto path = os.attr("path").attr("abspath")(os.attr("getcwd")());
    
        auto sys = py::module::import("sys");
        sys.attr("path").attr("insert")(0, path);
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: Appending current path to PyInterpreter");
    }
    
    std::string PythonScript::changeSlashesToDots(std::string script) {
        std::string rtn = engine::MAREngine::getEngine()->getAssetsPath() + script;
    
        size_t pos = rtn.find("/");
    
        while (pos != std::string::npos) {
            rtn.replace(pos, 1, ".");
            pos = rtn.find("/", pos + 1);
        }
    
        rtn = rtn.substr(0, rtn.size() - 3);
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: changing slashes {} to dots {}", script, rtn);
    
        return rtn;
    }
    
    std::string PythonScript::getModuleFromPath(std::string script) {
        const std::string filename = script.substr(script.find_last_of("/") + 1, script.size());
        const std::string deletedDotPyFromPath = filename.substr(0, filename.size() - 3);
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: returning module {} from path {}", script, rtn);
    
        return deletedDotPyFromPath;
    }


}
