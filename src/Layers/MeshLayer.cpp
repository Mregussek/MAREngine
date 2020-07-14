/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MeshLayer.h"


namespace mar {
	namespace layers {


		void MeshLayer::initialize() {
			m_mesh = new graphics::Mesh();
			m_renderer = new graphics::Renderer();

			m_mesh->create();
			m_renderer->create();
		}

		void MeshLayer::scene(graphics::SceneType scenetype, graphics::MeshType meshtype) {
			m_mesh->loadScene(&graphics::Scene(scenetype), meshtype);
			m_renderer->initialize(m_mesh->getLayout(), getShaderType(meshtype));
		}

		void MeshLayer::load() {
			m_renderer->initialize(m_mesh->getLayout(), getShaderType(m_mesh->getMeshType()));
		}

		void MeshLayer::set(const Ref<graphics::FrameBuffer>& framebuffer) {
			m_framebuffer = framebuffer;
		}

		void MeshLayer::prepareFrame() {
			if (storage::usegui) {
				m_framebuffer->bind();
				m_framebuffer->clear();
				m_framebuffer->unbind();
			}
		}

		void MeshLayer::update() {
			if (storage::usegui) {
				m_framebuffer->bind();

				m_renderer->draw(m_mesh);

				m_framebuffer->unbind();
			}
			else {
				m_renderer->draw(m_mesh);
			}
		}

		void MeshLayer::closeLayer() {
			m_renderer->close();

			delete m_renderer;
			delete m_mesh;
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


} }