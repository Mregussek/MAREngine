/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MeshLayer.h"


namespace mar {
	namespace layers {


		void MeshLayer::initializeLayer(graphics::Renderer* renderer, graphics::Mesh* mesh) {
			m_mesh = mesh;
			m_renderer = renderer;
		}

		void MeshLayer::create(const Ref<graphics::RendererFactory>& factory, const bool& usegui) {
			m_useGUI = usegui;
			m_renderer->createRenderer(factory, usegui);
			m_mesh->createMesh(factory);
		}

		void MeshLayer::scene(graphics::SceneType scenetype, graphics::MeshType meshtype) {
			m_mesh->loadScene(&graphics::Scene(scenetype), meshtype);
			m_renderer->initialize(m_mesh->getLayout(), getShaderType(meshtype));
		}

		void MeshLayer::load() {
			m_renderer->initialize(m_mesh->getLayout(), getShaderType(m_mesh->getMeshType()));
		}

		void MeshLayer::set(const gui::GUIData* guidata, const graphics::CameraData* cameradata) {
			m_renderer->setReferences(guidata, cameradata);
		}

		void MeshLayer::set(const graphics::CameraData* cameradata) {
			m_renderer->setReferences(cameradata);
		}

		void MeshLayer::set(const Ref<graphics::FrameBuffer>& framebuffer) {
			m_framebuffer = framebuffer;
		}

		void MeshLayer::prepareFrame() {
			if (m_useGUI) {
				m_framebuffer->bind();
				m_framebuffer->clear();
				m_framebuffer->unbind();
			}
		}

		void MeshLayer::update() {
			if (m_useGUI) {
				m_framebuffer->bind();

				m_renderer->draw(m_mesh);

				m_framebuffer->unbind();
			}
			else {
				m_renderer->draw(m_mesh);
			}
		}

		void MeshLayer::closeLayer() {
			m_renderer->closeRenderer();

			delete m_renderer;
			delete m_mesh;
		}

		graphics::ShaderType MeshLayer::getShaderType(graphics::MeshType meshtype) {
			if (m_renderer->useGUI()) {
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