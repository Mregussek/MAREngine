/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MeshLayer.h"
#include "../Debug/Log.h"


namespace mar {
	namespace layers {


		void MeshLayer::initialize() {
			m_renderer = new graphics::RendererOpenGL();

			m_mesh.create();

			MAR_CORE_INFO("MESH_LAYER: initialized");
		}

		void MeshLayer::load() {
			m_renderer->initialize(m_mesh.getLayout(), getShaderType(m_mesh.getMeshType()));

			MAR_CORE_INFO("MESH_LAYER: loaded");
		}

		void MeshLayer::prepareFrame() {

		}

		void MeshLayer::update() {
			m_renderer->draw(&m_mesh);
		}

		void MeshLayer::closeLayer() {
			m_renderer->close();

			delete m_renderer;

			MAR_CORE_INFO("MESH_LAYER: Closed layer!");
		}

		graphics::ShaderType MeshLayer::getShaderType(graphics::MeshType meshtype) {
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

		void MeshLayer::set(graphics::Light* light) { 
			m_renderer->setLight(light);
		}


} }