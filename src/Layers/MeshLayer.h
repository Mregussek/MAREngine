/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_LAYER_H
#define MAR_ENGINE_MESH_LAYER_H

#include "../mar.h"
#include "Layer.h"
#include "../Renderer/Mesh/Mesh.h"
#include "../Renderer/Renderer.h"


namespace mar {
	namespace layers {


		class MeshLayer : public Layer {
			const char* m_debugName;

			Ref<graphics::FrameBuffer> m_framebuffer;
			graphics::Mesh* m_mesh;
			graphics::Renderer* m_renderer;

		public:
			MeshLayer() = default;
			MeshLayer(const char* name) : m_debugName(name) { }

			void initialize();
			void scene(graphics::SceneType scenetype, graphics::MeshType meshtype);
			void load();

			// --- OVERLOADED METHODS --- //
			void prepareFrame() override;
			void update() override;
			void endFrame() override { }
			void closeLayer() override;

			// --- SET METHODS --- //
			void set(const Ref<graphics::FrameBuffer>& framebuffer);

			// --- GET METHODS --- //
			graphics::Mesh* getMesh() { return m_mesh; }
			graphics::ShaderType getShaderType(graphics::MeshType meshtype);

		};


} }

#endif // !MAR_ENGINE_MESH_LAYER_H