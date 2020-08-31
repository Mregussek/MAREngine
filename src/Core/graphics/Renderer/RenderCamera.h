/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_GRAPHICS_RENDER_CAMERA_H
#define MAR_ENGINE_GRAPHICS_RENDER_CAMERA_H

#include "../../../mar.h"


namespace mar {
	namespace graphics {


		struct RenderCamera {
			maths::mat4 projection;
			maths::mat4 view;
			maths::mat4 model;

			maths::mat4 mvp;

			maths::vec3 position;
		};




} }





#endif // !MAR_ENGINE_RENDER_CAMERA_H