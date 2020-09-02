/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

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
