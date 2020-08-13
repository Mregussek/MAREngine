/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "PythonScript.h"
#include "MAREnginePy.cpp"
#include "../ecs/ECS/Entity.h"
#include "../ecs/ECS/Components.h"
#include "../ecs/ECS/Systems.h"


namespace mar {
	namespace scripting {


        void PythonScript::loadScript(const char* from, const char* what) {
            if (initialized)
                scriptModule.reload();
            else 
                scriptModule = py::module::import(from);

            module = scriptModule.attr(what)();

            initialized = true;
        }

		void PythonScript::start(const ecs::Entity& e) {
            if (!initialized)
                return;

            module.attr("transform") = e.getComponent<ecs::TransformComponent>();

            if (e.hasComponent<ecs::LightComponent>()) {
                auto& light = e.getComponent<ecs::LightComponent>();
                module.attr("light") = light;
            }

            if (e.hasComponent<ecs::CameraComponent>()) {
                auto& camera = e.getComponent<ecs::CameraComponent>();
                module.attr("camera") = camera;
            }

            if (e.hasComponent<ecs::ColorComponent>()) {
                auto& color = e.getComponent<ecs::ColorComponent>();
                module.attr("color") = color;
            }

            module.attr("start")();
        }

        void PythonScript::update(ecs::Entity& e) {
            if (!initialized)
                return;

            module.attr("update")();

            auto& tran = e.getComponent<ecs::TransformComponent>();
            tran = module.attr("transform").cast<ecs::TransformComponent>();
            ecs::System::handleTransformComponent(tran);

            if (e.hasComponent<ecs::LightComponent>()) {
                auto& light = e.getComponent<ecs::LightComponent>();
                light = module.attr("light").cast<ecs::LightComponent>();
            }

            if (e.hasComponent<ecs::CameraComponent>()) {
                auto& camera = e.getComponent<ecs::CameraComponent>();
                camera = module.attr("camera").cast<ecs::CameraComponent>();
            }

            if (e.hasComponent<ecs::ColorComponent>()) {
                auto& color = e.getComponent<ecs::ColorComponent>();
                color = module.attr("color").cast<ecs::ColorComponent>();
            }
        }




} }