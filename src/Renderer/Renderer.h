/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"
#include "../GUI/GUI.h"
#include "Camera/Camera.h"
#include "RendererFactory.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshCreator.h"

namespace mar {
	namespace graphics {


		namespace constants {
			const size_t maxCount = 100000;
			const size_t maxVertexCount = maxCount * sizeof(Vertex);
			const size_t maxIndexCount = maxCount * 5;
			const size_t maxObjectsInScene = 32;
		}

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
			bool m_initialized = false;		// check, if renderer is initialized
			bool m_useGUI = false;	// check, which type of shader we want to use (we don't need gui calculations if it is not connected)
			static RendererStatistics s_stats;
			// --- References
			const CameraData* m_cameraData;
			const gui::GUIData* m_guiData;

		public:
			Renderer() = default;

			void createRenderer(const Ref<RendererFactory>& factory);
			void closeRenderer();

			void initialize(const std::vector<unsigned int>& layout, const ShaderType type);
			void setReferences(const gui::GUIData* guidata, const CameraData* cameradata);

			void draw(Mesh* mesh);

		private:
			void updateMeshData(Mesh* mesh);
			void updateGUIData();
			void updateCameraData();
			void updateLightData(Light* light);

			void bind();
			void unbind();

		public:
			inline static RendererStatistics& getStatistics() { return s_stats; }
		};


} }

#endif // !RENDERER_H