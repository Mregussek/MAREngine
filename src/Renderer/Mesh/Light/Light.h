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
			glm::vec3 _position{ 0.0f, 0.5f, 5.0f };
		public:
			Light() = default;

			const glm::vec3& getPosition() const { return _position; }
		};


} }

#endif //! MAR_ENGINE_LIGHT_H
