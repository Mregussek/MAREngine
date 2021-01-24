/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_PLATFORMS_FRAME_BUFFER_OPENGL_H
#define MAR_ENGINE_PLATFORMS_FRAME_BUFFER_OPENGL_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace marengine {


	class FramebufferOpenGL {
	public:

		FramebufferOpenGL() = default;

		void initialize(float width, float height);
		void close();

		void bind() const;
		void unbind() const;

		void clear(maths::vec3 backgroundColor) const;

		void resize(float x, float y);

		// --- GET METHODS --- // 

		uint32_t getColorAttach() const;
		uint32_t getDepthAttach() const;
		maths::vec2 getSize() const;
		
		// --- SET METHODS --- //

		void setSize(float x, float y);
		
	private:

		void createColorAttachment();
		void createDepthAttachment();
		void createFramebuffer();
		void checkCreationStatus();


		maths::vec2 m_size{ 0.f, 0.f };

		uint32_t m_id{ 0 };
		uint32_t m_colorAttachment{ 0 };
		uint32_t m_depthAttanchment{ 0 };

	};


}


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
