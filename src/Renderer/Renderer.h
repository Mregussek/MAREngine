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
			const size_t maxCount = 1000;
			const size_t maxVertexCount = maxCount * 10 * sizeof(Vertex);
			const size_t maxIndexCount = maxCount * 5;
			const size_t maxObjectsInScene = 32;
		}

		struct RendererStatistics {
			unsigned int _countOfDrawCalls;
			unsigned int _countOfVertices;
			unsigned int _countOfIndices;
			unsigned int _countOfShapes;
			unsigned int _countOfTriangles;

			RendererStatistics()
				:_countOfDrawCalls(0),
				_countOfVertices(0),
				_countOfIndices(0),
				_countOfShapes(0),
				_countOfTriangles(0)
			{}

			void resetStatistics() {
				_countOfDrawCalls = 0;
				_countOfIndices = 0;
				_countOfShapes = 0;
				_countOfVertices = 0;
				_countOfTriangles = 0;
			}
		};

		class Renderer {
			// --- Buffers
			Ref<VertexBuffer> m_vbo;
			Ref<VertexBufferLayout> m_layout;
			Ref<VertexArray> m_vao;
			Ref<ElementBuffer> m_ebo;
			Ref<Shader> m_mainShader;
			Ref<Shader> m_cubemapShader;

			// --- Knowledge about state of Renderer
			bool m_initialized = false;		// check, if renderer is initialized
			bool m_useGUI = false;	// check, which type of shader we want to use (we don't need gui calculations if it is not connected)
			RendererStatistics m_stats;

		public:
			Renderer() = default;

			void createRenderer(const Ref<RendererFactory>& factory);
			void closeRenderer();

			void initialize(const std::vector<unsigned int>& layout, const bool& useGUI);

			void draw(Mesh* mesh);

			void updateGUIData(Mesh* mesh, const gui::GUIData* guidata);
			void updateCameraData(const CameraData* cameradata);
			void updateLightData(Mesh* mesh);

			void bind();
			void unbind();

			const RendererStatistics& getStatistics() const { return m_stats; }
		};


} }

#endif // !RENDERER_H