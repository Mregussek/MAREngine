/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LIGHT_H
#define MAR_ENGINE_LIGHT_H


#include "../../../mar.h"

namespace mar {
    namespace graphics {


		class Light {
			glm::vec3 _position{ 0.0f, 0.0f, 0.0f };

			glm::vec3 _ambient{ 0.5f, 0.5f, 0.5f };
			glm::vec3 _diffuse{ 0.9f, 0.9f, 0.9f };
			glm::vec3 _specular{ 0.5f, 0.5f, 0.5f };

			glm::vec3 _ambientStrength{ 0.2f, 0.2f, 0.2f };
			glm::vec3 _diffuseStrength{ 0.7f, 0.7f, 0.7f };
			glm::vec3 _specularStrength{ 1.0f, 1.0f, 1.0f };

			float _constant{ 1.0f };
			float _linear{ 0.045f };
			float _quadratic{ 0.0075f };

			float _shininess{ 64.0f };

		public:
			Light() = default;

			void setPosition(const glm::vec3 new_pos) { _position = new_pos; }

			const glm::vec3& getPosition() const { return _position; }
			const glm::vec3& getAmbient() const { return _ambient; }
			const glm::vec3& getDiffuse() const { return _diffuse; }
			const glm::vec3& getSpecular() const { return _specular; }

			const glm::vec3& getAmbientStrength() const { return _ambientStrength; }
			const glm::vec3& geDiffuseStrength() const { return _diffuseStrength; }
			const glm::vec3& geSpecularStrength() const { return _specularStrength; }

			const float& getShininess() const { return _shininess; }

			const float& getConstant() const { return _constant; }
			const float& getLinear() const { return _linear; }
			const float& getQuadratic() const { return _quadratic; }
		};


} }

#endif //! MAR_ENGINE_LIGHT_H
