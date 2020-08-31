/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_RENDERER_INCLUDE_H
#define MAR_ENGINE_GRAPHICS_RENDERER_INCLUDE_H


#include "../../../Platform/OpenGL/ElementBufferOpenGL.h"
#include "../../../Platform/OpenGL/VertexArrayOpenGL.h"
#include "../../../Platform/OpenGL/VertexBufferOpenGL.h"
#include "../../../Platform/OpenGL/VertexBufferLayoutOpenGL.h"
#include "../../../Platform/OpenGL/FrameBufferOpenGL.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Platform/OpenGL/ShaderOpenGL.h"

#include "../../ecs/Scene.h"
#include "../../ecs/SceneManager.h"

#include "../GraphicsLogs.h"


namespace mar {
	namespace graphics {


		struct RendererStatistics {
			uint32_t drawCallsCount;
			uint32_t verticesCount;
			uint32_t indicesCount;
			uint32_t shapesCount;
			uint32_t trianglesCount;

			RendererStatistics()
				: drawCallsCount(0),
				verticesCount(0),
				indicesCount(0),
				shapesCount(0),
				trianglesCount(0)
			{}

			void resetStatistics() {
				drawCallsCount = 0;
				indicesCount = 0;
				shapesCount = 0;
				verticesCount = 0;
				trianglesCount = 0;
			}
		};


} }

#endif // !MAR_ENGINE_RENDERER_INCLUDE_H
