/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_SCRIPTING_ENGINE_EMBED_HELPERS_H
#define MAR_ENGINE_SCRIPTING_ENGINE_EMBED_HELPERS_H


#include "../../mar.h"


namespace mar {
	namespace scripting {


		class Transform {
		public:
			maths::vec3 center;
			maths::vec3 scale;
			maths::vec3 angles;
		};

		class Light {
		public:
			maths::vec3 ambient;
			maths::vec3 diffuse;
			maths::vec3 specular;

			float constant;
			float linear;
			float quadratic;

			float shininess;
		};

		class Camera {
		public:
			float p_fov;
			float p_aspectRatio;
			float p_near;
			float p_far;

			float o_left;
			float o_right;
			float o_top;
			float o_bottom;
			float o_near;
			float o_far;
		};

		class Color {
		public:
			maths::vec3 texture;
		};


		class PyEntity {
		public:
			Transform transform;
			Light light;
			Camera camera;
			Color color;

			virtual void start() { }
			virtual void update() { }
		};

		class PyTrampoline : public PyEntity {
		public:
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
