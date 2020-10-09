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


    PythonScript::PythonScript()
        : m_initialized(false)
    {}
    
    void PythonScript::loadScript(std::string path_to_script) {
        std::string from = changeSlashesToDots(path_to_script);
        std::string what = getModuleFromPath(path_to_script);
    
        if (m_initialized) { m_scriptModule.reload(); }
        else { 
            m_scriptModule = py::module::import(from.c_str()); 
            m_initialized = true;
        }
    
        m_module = m_scriptModule.attr(what.c_str())();
    
        SCRIPTING_INFO("PYTHON_SCRIPT: Loaded script {} from {}", what, from);
    }
    
    void PythonScript::start(const ecs::Entity* e) {
        if (!m_initialized)
            return;
    
        m_module.attr("transform") = e->getComponent<ecs::TransformComponent>();
    
        if (e->hasComponent<ecs::LightComponent>()) {
            auto& light = e->getComponent<ecs::LightComponent>();
            m_module.attr("light") = light;
        }
    
        if (e->hasComponent<ecs::CameraComponent>()) {
            auto& camera = e->getComponent<ecs::CameraComponent>();
            m_module.attr("camera") = camera;
        }
    
        if (e->hasComponent<ecs::ColorComponent>()) {
            auto& color = e->getComponent<ecs::ColorComponent>();
            m_module.attr("color") = color;
        }
    
        m_module.attr("start")();
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: Calling start method at python script");
    }
    
    void PythonScript::update(ecs::Entity* e) {
        if (!m_initialized)
            return;
    
        m_module.attr("transform") = e->getComponent<ecs::TransformComponent>();
    
        if (e->hasComponent<ecs::LightComponent>()) {
            auto& light = e->getComponent<ecs::LightComponent>();
            m_module.attr("light") = light;
        }
    
        if (e->hasComponent<ecs::CameraComponent>()) {
            auto& camera = e->getComponent<ecs::CameraComponent>();
            m_module.attr("camera") = camera;
        }
    
        if (e->hasComponent<ecs::ColorComponent>()) {
            auto& color = e->getComponent<ecs::ColorComponent>();
            m_module.attr("color") = color;
        }
    
        m_module.attr("update")();
    
        auto& tran = e->getComponent<ecs::TransformComponent>();
        tran = m_module.attr("transform").cast<ecs::TransformComponent>();
        tran.recalculate();
    
        if (e->hasComponent<ecs::LightComponent>()) {
            auto& light = e->getComponent<ecs::LightComponent>();
            light = m_module.attr("light").cast<ecs::LightComponent>();
        }
    
        if (e->hasComponent<ecs::CameraComponent>()) {
            auto& camera = e->getComponent<ecs::CameraComponent>();
            camera = m_module.attr("camera").cast<ecs::CameraComponent>();
        }
    
        if (e->hasComponent<ecs::ColorComponent>()) {
            auto& color = e->getComponent<ecs::ColorComponent>();
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
        std::string rtn = script.substr(script.find_last_of("/") + 1, script.size());
        rtn = rtn.substr(0, rtn.size() - 3);
    
        SCRIPTING_TRACE("PYTHON_SCRIPT: returning module {} from path {}", script, rtn);
    
        return rtn;
    }


}
