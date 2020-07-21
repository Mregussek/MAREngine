/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LIGHT_H
#define MAR_ENGINE_LIGHT_H


#include "../../mar.h"

namespace mar {
    namespace graphics {


		class Light {
			maths::vec3 _position{ 0.0f, 0.0f, 10.0f };

			maths::vec3 _ambient{ 0.5f, 0.5f, 0.5f };
			maths::vec3 _diffuse{ 0.9f, 0.9f, 0.9f };
			maths::vec3 _specular{ 0.5f, 0.5f, 0.5f };

			maths::vec3 _ambientStrength{ 0.2f, 0.2f, 0.2f };
			maths::vec3 _diffuseStrength{ 0.7f, 0.7f, 0.7f };
			maths::vec3 _specularStrength{ 1.0f, 1.0f, 1.0f };

			float _constant{ 1.0f };
			float _linear{ 0.045f };
			float _quadratic{ 0.0075f };

			float _shininess{ 64.0f };

		public:
			Light() = default;

			void setPosition(const maths::vec3 new_pos) { _position = new_pos; }

			inline maths::vec3& getPosition() { return _position; }
			inline maths::vec3& getAmbient() { return _ambient; }
			inline maths::vec3& getDiffuse() { return _diffuse; }
			inline maths::vec3& getSpecular() { return _specular; }
			
			inline maths::vec3& getAmbientStrength() { return _ambientStrength; }
			inline maths::vec3& getDiffuseStrength() { return _diffuseStrength; }
			inline maths::vec3& getSpecularStrength() { return _specularStrength; }
			 
			inline float& getShininess() { return _shininess; }
			
			inline float& getConstant() { return _constant; }
			inline float& getLinear() { return _linear; }
			inline float& getQuadratic() { return _quadratic; }
		};


} }

#endif //! MAR_ENGINE_LIGHT_H
