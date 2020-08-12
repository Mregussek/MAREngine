/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_ECS_SYSTEMS_H
#define MAR_ENGINE_ECS_SYSTEMS_H

#include "../../../mar.h"
#include "../ECSLogs.h"
#include "Components.h"


namespace mar {
	namespace ecs {


		class System {
		public:

			static void handleTransformComponent(TransformComponent& tran) {
				using namespace maths;

				tran.transform =
					mat4::translation(tran.center) *
					mat4::rotation(Trig::toRadians(tran.angles.x), { 1.f, 0.f, 0.f }) *
					mat4::rotation(Trig::toRadians(tran.angles.y), { 0.f, 1.f, 0.f }) *
					mat4::rotation(Trig::toRadians(tran.angles.z), { 0.f, 0.f, 1.f }) *
					mat4::scale(tran.scale);

				ECS_TRACE("SYSTEM: calculated new TransformComponent!");
			}

			static std::string changeSlashesToDots(std::string str) {
				size_t pos = str.find("/");

				while (pos != std::string::npos) {
					str.replace(pos, 1, ".");
					pos = str.find("/", pos + 1);
				}

				return str.substr(0, str.size() - 3);
			}

			static std::string getModuleFromPath(std::string str) {
				str = str.substr(str.find_last_of("/") + 1, str.size());
				return str.substr(0, str.size() - 3);
			}

		};


} }

#endif // !MAR_ENGINE_ECS_SYSTEMS_H
