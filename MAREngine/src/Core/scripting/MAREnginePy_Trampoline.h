/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_SCRIPTING_ENGINE_TRAMPOLINE_H
#define MAR_ENGINE_SCRIPTING_ENGINE_TRAMPOLINE_H


#include "../../mar.h"
#include "../ecs/ECS/Components.h"
#include "../ecs/ECS/Entity.h"

namespace mar {
	namespace scripting {


		class PyEntity {
		public:
			ecs::TransformComponent transform;
			ecs::LightComponent light;
			ecs::CameraComponent camera;
			ecs::ColorComponent color;

			virtual void start() { }
			virtual void update() { }
		};

		class PyTrampoline : public PyEntity {
		public:
			using PyEntity::PyEntity;

			void start() override {
				PYBIND11_OVERLOAD(
					void,
					PyEntity,
					start
				);
			}

			void update() override {
				PYBIND11_OVERLOAD(
					void,
					PyEntity,
					update
				);
			}
		};


} }


#endif // !MAR_ENGINE_SCRIPTING_ENGINE_EMBED_HELPERS_H
