/**
 *				MAREngine - open source 3D game engine
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
#include "../ecs/ECS/Entity.h"
#include "../ecs/ECS/Components.h"
#include "../ecs/ECS/Systems.h"


namespace mar {
	namespace scripting {


        PythonScript::PythonScript()
            : initialized(false)
        {}

        void PythonScript::loadScript(const char* from, const char* what) {
            if (initialized)
                scriptModule.reload();
            else 
                scriptModule = py::module::import(from);

            module = scriptModule.attr(what)();

            initialized = true;

            SCRIPTING_INFO("PYTHON_SCRIPT: Loaded script {} from {}", what, from);
        }

		void PythonScript::start(const ecs::Entity* e) {
            if (!initialized)
                return;

            module.attr("transform") = e->getComponent<ecs::TransformComponent>();

            if (e->hasComponent<ecs::LightComponent>()) {
                auto& light = e->getComponent<ecs::LightComponent>();
                module.attr("light") = light;
            }

            if (e->hasComponent<ecs::CameraComponent>()) {
                auto& camera = e->getComponent<ecs::CameraComponent>();
                module.attr("camera") = camera;
            }

            if (e->hasComponent<ecs::ColorComponent>()) {
                auto& color = e->getComponent<ecs::ColorComponent>();
                module.attr("color") = color;
            }

            module.attr("start")();

            SCRIPTING_TRACE("PYTHON_SCRIPT: Calling start method at python script");
        }

        void PythonScript::update(ecs::Entity* e) {
            if (!initialized)
                return;

            module.attr("transform") = e->getComponent<ecs::TransformComponent>();

            if (e->hasComponent<ecs::LightComponent>()) {
                auto& light = e->getComponent<ecs::LightComponent>();
                module.attr("light") = light;
            }

            if (e->hasComponent<ecs::CameraComponent>()) {
                auto& camera = e->getComponent<ecs::CameraComponent>();
                module.attr("camera") = camera;
            }

            if (e->hasComponent<ecs::ColorComponent>()) {
                auto& color = e->getComponent<ecs::ColorComponent>();
                module.attr("color") = color;
            }

            module.attr("update")();

            auto& tran = e->getComponent<ecs::TransformComponent>();
            tran = module.attr("transform").cast<ecs::TransformComponent>();
            ecs::System::handleTransformComponent(tran);

            if (e->hasComponent<ecs::LightComponent>()) {
                auto& light = e->getComponent<ecs::LightComponent>();
                light = module.attr("light").cast<ecs::LightComponent>();
            }

            if (e->hasComponent<ecs::CameraComponent>()) {
                auto& camera = e->getComponent<ecs::CameraComponent>();
                camera = module.attr("camera").cast<ecs::CameraComponent>();
            }

            if (e->hasComponent<ecs::ColorComponent>()) {
                auto& color = e->getComponent<ecs::ColorComponent>();
                color = module.attr("color").cast<ecs::ColorComponent>();
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


} }