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


#include "ContextSDL.h"


namespace marengine {


	SDL_GLContext ContextSDL::s_context;
	bool ContextSDL::windowGoingToClose{ false };


	bool ContextSDL::create(SDL_Window* window) {
		s_context = SDL_GL_CreateContext(window);
		const int32_t isCurrentContext_OK = SDL_GL_MakeCurrent(window, s_context);
		if (isCurrentContext_OK == SDL_TRUE) {
			return false;
		}

		return true;
	}

	void ContextSDL::destroy() {
		SDL_GL_DeleteContext(s_context);
	}

	SDL_GLContext* ContextSDL::getContext() {
		return &s_context; 
	}



}
