/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MeshLayer.h"
#include "../../Debug/Log.h"
#include "../../Core/Renderer/RendererOpenGL.h"
#include "../../Core/Mesh/Mesh.h"


namespace mar {
	namespace layers {


		LayerMesh::LayerMesh(const char* name) 
			: m_debugName(name),
			m_renderer(nullptr),
			m_mesh(nullptr)
		{ }

		void LayerMesh::initialize() {
			m_renderer = new graphics::RendererOpenGL();
			m_mesh = new graphics::Mesh();

			m_mesh->create();

			MAR_CORE_INFO("MESH_LAYER: initialized");
		}

		void LayerMesh::load() {
			m_renderer->initialize(m_mesh->getLayout(), getShaderType(m_mesh->getMeshType()));

			MAR_CORE_INFO("MESH_LAYER: loaded");
		}

		void LayerMesh::update() {
			m_renderer->draw(m_mesh);
		}

		void LayerMesh::closeLayer() {
			m_renderer->close();

			delete m_mesh;
			delete m_renderer;

			MAR_CORE_INFO("MESH_LAYER: Closed layer!");
		}

		graphics::ShaderType LayerMesh::getShaderType(graphics::MeshType meshtype) {
			if (storage::usegui) {
				switch (meshtype) {
				case NORMAL_MESH_TYPE:
				case OBJECTS_MESH_TYPE:
					return SHADER_DEFAULT; break;
				case CUBEMAPS_MESH_TYPE:
					return SHADER_CUBEMAP; break;
				default:
					MAR_CORE_ERROR("Trying to iniitialize shader with GUI to Renderer without GUI!");
					MAR_CORE_ERROR("Cannot initialize Renderer!");
					return SHADER_DEFAULT;
				}
			}
			else {
				switch (meshtype) {
				case NORMAL_MESH_TYPE:
				case OBJECTS_MESH_TYPE:
					return SHADER_WITHOUT_GUI; break;
				case CUBEMAPS_MESH_TYPE:
					return SHADER_CUBEMAP_WITHOUT_GUI; break;
				default:
					MAR_CORE_ERROR("Trying to iniitialize shader without GUI to Renderer with GUI!");
					MAR_CORE_ERROR("Cannot initialize Renderer!");
					return SHADER_WITHOUT_GUI;
				}
			}
		}

		graphics::RendererOpenGL* LayerMesh::getRenderer() { 
			return m_renderer; 
		}

		graphics::Mesh* LayerMesh::getMesh() { 
			return m_mesh; 
		}


} }