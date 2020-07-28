/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_ECS_SYSTEMS_H
#define MAR_ENGINE_ECS_SYSTEMS_H

#include "../../../mar.h"
#include "../../../Debug/Log.h"
#include "../Component/Components.h"


namespace mar {
	namespace ecs {


		class System {
		public:

			static void handleTransformComponent(TransformComponent& tran) {
				using namespace maths;

				tran.transform =
					mat4::translation(tran.center) *
					mat4::rotation(maths::Trig::toRadians(tran.angles.x), { 1.f, 0.f, 0.f }) *
					mat4::rotation(maths::Trig::toRadians(tran.angles.y), { 0.f, 1.f, 0.f }) *
					mat4::rotation(maths::Trig::toRadians(tran.angles.z), { 0.f, 0.f, 1.f }) *
					mat4::scale(tran.scale);
			}


		};


} }

#endif // !MAR_ENGINE_ECS_SYSTEMS_H
