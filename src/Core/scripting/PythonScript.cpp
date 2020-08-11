/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "PythonScript.h"
#include "../ecs/ECS/Entity.h"
#include "../ecs/ECS/Components.h"
#include "../ecs/ECS/Systems.h"


namespace mar {
	namespace scripting {


        void PythonScript::loadScript(const char* from, const char* what) {
            appendCurrentPath();

            scriptModule = py::module::import(from);
            module = scriptModule.attr(what)();

            initialized = true;
        }

		void PythonScript::start(const ecs::Entity& e) {
            if (!initialized)
                return;

            
            if (e.hasComponent<ecs::TransformComponent>()) {
                auto& tran = e.getComponent<ecs::TransformComponent>();
                module.attr("transform").attr("center").attr("x") = tran.center.x;
                module.attr("transform").attr("center").attr("y") = tran.center.y;
                module.attr("transform").attr("center").attr("z") = tran.center.z;

                module.attr("transform").attr("angles").attr("x") = tran.angles.x;
                module.attr("transform").attr("angles").attr("y") = tran.angles.y;
                module.attr("transform").attr("angles").attr("z") = tran.angles.z;

                module.attr("transform").attr("scale").attr("x") = tran.scale.x;
                module.attr("transform").attr("scale").attr("y") = tran.scale.y;
                module.attr("transform").attr("scale").attr("z") = tran.scale.z;
            }

            if (e.hasComponent<ecs::LightComponent>()) {
                auto& light = e.getComponent<ecs::LightComponent>();
                module.attr("light").attr("ambient").attr("x") = light.ambient.x;
                module.attr("light").attr("ambient").attr("y") = light.ambient.y;
                module.attr("light").attr("ambient").attr("z") = light.ambient.z;

                module.attr("light").attr("diffuse").attr("x") = light.diffuse.x;
                module.attr("light").attr("diffuse").attr("y") = light.diffuse.y;
                module.attr("light").attr("diffuse").attr("z") = light.diffuse.z;

                module.attr("light").attr("specular").attr("x") = light.specular.x;
                module.attr("light").attr("specular").attr("y") = light.specular.y;
                module.attr("light").attr("specular").attr("z") = light.specular.z;

                module.attr("light").attr("constant") = light.constant;
                module.attr("light").attr("linear") = light.linear;
                module.attr("light").attr("quadratic") = light.quadratic;
                module.attr("light").attr("shininess") = light.shininess;
            }

            if (e.hasComponent<ecs::ColorComponent>()) {
                auto& color = e.getComponent<ecs::ColorComponent>();
                module.attr("color").attr("texture").attr("x") = color.texture.x;
                module.attr("color").attr("texture").attr("y") = color.texture.y;
                module.attr("color").attr("texture").attr("z") = color.texture.z;
            }

            module.attr("start")();
        }

        void PythonScript::update(ecs::Entity& e) {
            if (!initialized)
                return;

            module.attr("update")();

            if (e.hasComponent<ecs::TransformComponent>()) {
                auto& tran = e.getComponent<ecs::TransformComponent>();

                tran.center.x = module.attr("transform").attr("center").attr("x").cast<float>(); 
                tran.center.y = module.attr("transform").attr("center").attr("y").cast<float>();
                tran.center.z = module.attr("transform").attr("center").attr("z").cast<float>();
                         
                tran.angles.x = module.attr("transform").attr("angles").attr("x").cast<float>();
                tran.angles.y = module.attr("transform").attr("angles").attr("y").cast<float>();
                tran.angles.z = module.attr("transform").attr("angles").attr("z").cast<float>();
                    
                tran.scale.x = module.attr("transform").attr("scale").attr("x").cast<float>();
                tran.scale.y = module.attr("transform").attr("scale").attr("y").cast<float>();
                tran.scale.z = module.attr("transform").attr("scale").attr("z").cast<float>();

                ecs::System::handleTransformComponent(tran);
            }

            if (e.hasComponent<ecs::LightComponent>()) {
                auto& light = e.getComponent<ecs::LightComponent>();
                light.ambient.x  = module.attr("light").attr("ambient").attr("x").cast<float>();
                light.ambient.y  = module.attr("light").attr("ambient").attr("y").cast<float>();
                light.ambient.z  = module.attr("light").attr("ambient").attr("z").cast<float>();
                          
                light.diffuse.x  = module.attr("light").attr("diffuse").attr("x").cast<float>();
                light.diffuse.y  = module.attr("light").attr("diffuse").attr("y").cast<float>();
                light.diffuse.z  = module.attr("light").attr("diffuse").attr("z").cast<float>();
                            
                light.specular.x = module.attr("light").attr("specular").attr("x").cast<float>();
                light.specular.y = module.attr("light").attr("specular").attr("y").cast<float>();
                light.specular.z = module.attr("light").attr("specular").attr("z").cast<float>();

                light.constant  = module.attr("light").attr("constant").cast<float>();
                light.linear    = module.attr("light").attr("linear").cast<float>();
                light.quadratic = module.attr("light").attr("quadratic").cast<float>();
                light.shininess = module.attr("light").attr("shininess").cast<float>();
            }

            if (e.hasComponent<ecs::ColorComponent>()) {
                auto& color = e.getComponent<ecs::ColorComponent>();
                color.texture.x = module.attr("color").attr("texture").attr("x").cast<float>();
                color.texture.y = module.attr("color").attr("texture").attr("y").cast<float>();
                color.texture.z = module.attr("color").attr("texture").attr("z").cast<float>();
            }
        }




} }