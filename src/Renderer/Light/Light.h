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

			const maths::vec3& getPosition() const { return _position; }
			const maths::vec3& getAmbient() const { return _ambient; }
			const maths::vec3& getDiffuse() const { return _diffuse; }
			const maths::vec3& getSpecular() const { return _specular; }
		
			const maths::vec3& getAmbientStrength() const { return _ambientStrength; }
			const maths::vec3& geDiffuseStrength() const { return _diffuseStrength; }
			const maths::vec3& geSpecularStrength() const { return _specularStrength; }

			const float& getShininess() const { return _shininess; }

			const float& getConstant() const { return _constant; }
			const float& getLinear() const { return _linear; }
			const float& getQuadratic() const { return _quadratic; }
		};


} }

#endif //! MAR_ENGINE_LIGHT_H
