/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"
#include "RendererFactory.h"
#include "../GUI/GUI.h"
#include "Camera/Camera.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshCreator.h"

namespace mar {
	namespace graphics {


		struct RendererStatistics {
			unsigned int drawCallsCount;
			unsigned int verticesCount;
			unsigned int indicesCount;
			unsigned int shapesCount;
			unsigned int trianglesCount;

			RendererStatistics()
				:drawCallsCount(0),
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

		class Renderer {
			// --- Buffers
			Ref<VertexBuffer> m_vbo;
			Ref<VertexBufferLayout> m_layout;
			Ref<VertexArray> m_vao;
			Ref<ElementBuffer> m_ebo;
			Ref<Shader> m_mainShader;
			// --- Knowledge about state of Renderer
			bool m_initialized = false;	// check, if renderer is initialized
			bool m_useGUI = false;		// check, which type of shader we want to use (we don't need gui calculations if it is not connected)
			static RendererStatistics s_stats;
			// --- References
			const CameraData* m_cameraData;
			const gui::GUIData* m_guiData;

		public:
			Renderer() = default;

			void createRenderer(const Ref<RendererFactory>& factory, const bool& usegui);
			void closeRenderer();

			void initialize(const std::vector<unsigned int>& layout, const ShaderType type);

			void draw(Mesh* mesh);

		private:
			void updateMeshData(Mesh* mesh);
			void updateGUIData();
			void updateCameraData();
			void updateLightData(Light* light);

			void bind();
			void unbind();

		public:
			// --- SET METHODS --- //
			void setReferences(const gui::GUIData* guidata, const CameraData* cameradata);
			void setReferences(const CameraData* cameradata);

			// --- GET METHODS --- // 
			inline static RendererStatistics& getStatistics() { return s_stats; }
			inline const bool useGUI() const { return m_useGUI; }
		};


} }

#endif // !RENDERER_H