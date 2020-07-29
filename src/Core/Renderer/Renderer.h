/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_INCLUDE_H
#define MAR_ENGINE_RENDERER_INCLUDE_H

#include "../../Editor/GUI/GUI.h"

#include "../Camera/Camera.h"

#include "../Mesh/Shapes/ShapeManipulator.h"
#include "../Mesh/MeshCreator.h"

#include "Buffers/ElementBuffer/ElementBuffer.h"
#include "Buffers/ElementBuffer/ElementBufferOpenGL.h"

#include "Buffers/VertexArray/VertexArray.h"
#include "Buffers/VertexArray/VertexArrayOpenGL.h"

#include "Buffers/VertexBuffer/VertexBuffer.h"
#include "Buffers/VertexBuffer/VertexBufferOpenGL.h"

#include "Buffers/VertexBuffer/VertexBufferLayout.h"
#include "Buffers/VertexBuffer/VertexBufferLayoutOpenGL.h"

#include "Buffers/FrameBuffer/FrameBuffer.h"
#include "Buffers/FrameBuffer/FrameBufferOpenGL.h"

#include "../Mesh/Texture/Texture.h"
#include "../Mesh/Texture/TextureOpenGL.h"

#include "../Shader/Shader.h"
#include "../Shader/ShaderOpenGL.h"

#include "../Scene/Scene.h"

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
