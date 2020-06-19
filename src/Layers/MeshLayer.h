/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "../mar.h"
#include "Layer.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Mesh/Mesh.h"


namespace mar {
	namespace layers {


		class MeshLayer : public Layer {
			std::string m_debugName;

			Ref<graphics::FrameBuffer> m_framebuffer;
			graphics::Mesh* m_mesh;
			graphics::Renderer* m_renderer;

		public:
			MeshLayer() = default;
			MeshLayer(std::string name) : m_debugName(name) { }

			void initializeLayer(graphics::Renderer* renderer, graphics::Mesh* mesh);
			void create(const Ref<graphics::RendererFactory>& factory, const bool& usegui);
			void scene(graphics::SceneType scenetype, graphics::MeshType meshtype);

			// --- OVERLOADED METHODS --- //
			void prepareFrame() override;
			void update() override;
			void endFrame() override { }
			void closeLayer() override;

			// --- SET METHODS --- //
			void set(const gui::GUIData* guidata, const graphics::CameraData* cameradata);
			void set(graphics::CameraData* cameradata);
			void set(const Ref<graphics::FrameBuffer>& framebuffer);

			// --- GET METHODS --- //
			graphics::Mesh* getMesh() { return m_mesh; }

		};


} }
