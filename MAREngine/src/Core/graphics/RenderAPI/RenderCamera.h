/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAR_ENGINE_GRAPHICS_RENDER_CAMERA_H
#define MAR_ENGINE_GRAPHICS_RENDER_CAMERA_H


#include "../../../mar.h"


namespace mar::ecs {
	struct TransformComponent;
	struct CameraComponent;
}
namespace mar::graphics {


	class RenderCamera{

		typedef maths::trig trig;
		typedef maths::vec3 vec3;
		typedef maths::mat4 mat4;

	public:
		
		RenderCamera() = default;

		void calculatePerspective(float zoom, float aspectRatio, float nearPlane, float farPlane);
		void calculateOrthographic(float left, float right, float top, float bottom, float nearPlane, float farPlane);
		void calculateView(maths::vec3 position, maths::vec3 lookAt, maths::vec3 up);
		void calculateModel(maths::vec3 arg);
		void recalculateMVP();

		void calculateCameraTransforms(const ecs::TransformComponent& transform, const ecs::CameraComponent& camera);

		const maths::mat4& getProjection() const { return m_projection; }
		const maths::mat4& getView() const { return m_view; }
		const maths::mat4& getModel() const { return m_model; }
		const maths::mat4& getMVP() const { return m_mvp; }
		const maths::vec3& getPosition() const { return m_position; }

	private:

		maths::mat4 m_model;
		maths::mat4 m_view;
		maths::mat4 m_projection;
		
		maths::mat4 m_mvp;

		maths::vec3 m_position;
	};




}


#endif // !MAR_ENGINE_RENDER_CAMERA_H