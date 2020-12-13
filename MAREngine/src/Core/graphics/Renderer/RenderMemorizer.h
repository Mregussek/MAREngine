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


#ifndef MAR_ENGINE_GRAPHICS_RENDERER_RENDER_MEMORIZER_H
#define MAR_ENGINE_GRAPHICS_RENDERER_RENDER_MEMORIZER_H


#include "../../../mar.h"


namespace mar::layers { class RenderLayer; }
namespace mar::platforms { class ShaderBufferStorageOpenGL; }
namespace mar::graphics {


	struct RenderMemorizer {

		static RenderMemorizer* Instance;

		uint32_t cameraSSBO{ 0 };

	};


}


#endif // !MAR_ENGINE_GRAPHICS_RENDERER_RENDER_MEMORIZER_H

