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


#ifndef MAR_ENGINE_EDITOR_GUI_GRAPHICS_H
#define MAR_ENGINE_EDITOR_GUI_GRAPHICS_H


#include "../../mar.h"
#include "../../Core/ecs/ECS/Components.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../Platform/OpenGL/PipelineOpenGL.h"
#include "../../Platform/OpenGL/ShaderOpenGL.h"


namespace mar {
	namespace editor {

		
		class GUI_Graphics {
			static GUI_Graphics s_instance;

			platforms::PipelineOpenGL m_pipeline;
			platforms::ShaderOpenGL m_shader;

		public:

			static GUI_Graphics& getInstance() { return s_instance; }

			void initialize();
			void close();

			void drawSelectedEntity(ecs::RenderableComponent& ren, ecs::TransformComponent& tran);

			void passToDrawEntity(ecs::Entity* e, bool ability_to_draw);
		};


} }


#endif // !MAR_ENGINE_EDITOR_GUI_GRAPHICS_H
