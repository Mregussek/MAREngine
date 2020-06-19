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
			m_renderer->createRenderer(factory, usegui);
			m_mesh->createMesh(factory);
		}

		void MeshLayer::scene(graphics::SceneType scenetype, graphics::MeshType meshtype) {
			graphics::ShaderType shader;

			if (m_renderer->useGUI()) {
				switch (meshtype) {
				case NORMAL_MESH_TYPE:
				case OBJECTS_MESH_TYPE:
					shader = SHADER_DEFAULT; break;
				case CUBEMAPS_MESH_TYPE:
					shader = SHADER_CUBEMAP; break;
				default:
					MAR_CORE_ERROR("Trying to iniitialize shader with GUI to Renderer without GUI!");
					MAR_CORE_ERROR("Cannot initialize Renderer!");
					return;
				}
			}
			else {
				switch (meshtype) {
				case NORMAL_MESH_TYPE:
				case OBJECTS_MESH_TYPE:
					shader = SHADER_WITHOUT_GUI; break;
				case CUBEMAPS_MESH_TYPE:
					shader = SHADER_CUBEMAP_WITHOUT_GUI; break;
				default:
					MAR_CORE_ERROR("Trying to iniitialize shader without GUI to Renderer with GUI!");
					MAR_CORE_ERROR("Cannot initialize Renderer!");
					return;
				}
			}

			m_mesh->loadScene(&graphics::Scene(scenetype), meshtype);
			m_renderer->initialize(m_mesh->getLayout(), shader);
		}

		void MeshLayer::set(const gui::GUIData* guidata, const graphics::CameraData* cameradata) {
			m_renderer->setReferences(guidata, cameradata);
		}

		void MeshLayer::set(graphics::CameraData* cameradata) {
			m_renderer->setReferences(cameradata);
		}

		void MeshLayer::set(const Ref<graphics::FrameBuffer>& framebuffer) {
			m_framebuffer = framebuffer;
		}

		void MeshLayer::prepareFrame() {
			m_framebuffer->bind();
			m_framebuffer->clear();
			m_framebuffer->unbind();
		}

		void MeshLayer::update() {
			m_framebuffer->bind();

			m_renderer->draw(m_mesh);

			m_framebuffer->unbind();
		}

		void MeshLayer::closeLayer() {
			m_renderer->closeRenderer();
		}



} }