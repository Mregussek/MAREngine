/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_LAYER_H
#define MAR_ENGINE_MESH_LAYER_H

#include "../../mar.h"
#include "../Layer.h"
#include "../../Core/Shader/Shader.h"


namespace mar {
	namespace graphics { class RendererOpenGL; class Mesh; enum class MeshType; }

	namespace layers {


		class LayerMesh : public Layer {
			const char* m_debugName;
			graphics::Mesh* m_mesh;
			graphics::RendererOpenGL* m_renderer;

		public:
			LayerMesh() = default;
			LayerMesh(const char* name);

			void initialize();
			void load();

			// --- OVERLOADED METHODS --- //

			void update() override;
			void closeLayer() override;


			// --- GET METHODS --- //

			graphics::RendererOpenGL* getRenderer();
			graphics::Mesh* getMesh();

			graphics::ShaderType getShaderType(graphics::MeshType meshtype);

		};


} }

#endif // !MAR_ENGINE_MESH_LAYER_H