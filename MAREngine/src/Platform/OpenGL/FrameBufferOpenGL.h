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


#ifndef MAR_ENGINE_PLATFORMS_FRAME_BUFFER_OPENGL_H
#define MAR_ENGINE_PLATFORMS_FRAME_BUFFER_OPENGL_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar::platforms {


	struct FrameBufferSpecification {
		maths::vec3 backgroundColor{ 0.22f, 0.69f, 0.87f };
		float width{ 0.f };
		float height{ 0.f };
		uint32_t samples = 1;

		bool swapChainTarget = false;

		FrameBufferSpecification() = default;
		FrameBufferSpecification(float w, float h) : width(w), height(h) { }
	};


	class FrameBufferOpenGL {
	public:

		FrameBufferOpenGL() = default;

		void initialize(const FrameBufferSpecification& spec);
		void close();

		void bind() const;
		void unbind() const;

		void clear() const;

		// --- GET METHODS --- // 

		const uint32_t& getColorAttach() const { return m_colorAttachment; }
		const uint32_t& getDepthAttach() const { return m_depthAttanchment; }
		FrameBufferSpecification& getSpecification() { return m_specification; }
		
		// --- SET METHODS --- //

		void setBackgroundColor(maths::vec3 background_color) { m_specification.backgroundColor = background_color; }
		
	private:

		void createColorAttachment();
		void createDepthAttachment();
		void createFramebuffer();
		void checkCreationStatus();

		uint32_t m_id{ 0 };
		uint32_t m_colorAttachment{ 0 };
		uint32_t m_depthAttanchment{ 0 };

		FrameBufferSpecification m_specification;
	};


}


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
