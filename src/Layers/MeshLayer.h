/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_LAYER_H
#define MAR_ENGINE_MESH_LAYER_H

#include "../mar.h"
#include "Layer.h"
#include "../Core/Mesh/Mesh.h"
#include "../Core/Renderer/Renderer.h"
#include "../Core/Renderer/Buffers/FrameBuffer/FrameBuffer.h"
#include "../Core/Renderer/Buffers/FrameBuffer/FrameBufferOpenGL.h"
#include "../Core/Shader/Shader.h"


namespace mar {
	namespace layers {


		class MeshLayer : public Layer {
			const char* m_debugName;

			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			graphics::Mesh m_mesh;
			graphics::RendererOpenGL* m_renderer;

		public:
			MeshLayer(const char* name) : m_debugName(name) { }

			void initialize();
			void load();

			// --- OVERLOADED METHODS --- //
			void prepareFrame() override;
			void update() override;
			void endFrame() override { }
			void closeLayer() override;

			// --- SET METHODS --- //
			void set(graphics::Light* light);
			void set(const graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer);

			// --- GET METHODS --- //
			graphics::Mesh* getMesh() { return &m_mesh; }
			graphics::ShaderType getShaderType(graphics::MeshType meshtype);

		};


} }

#endif // !MAR_ENGINE_MESH_LAYER_H