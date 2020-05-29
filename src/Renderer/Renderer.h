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

namespace mar {

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
		std::shared_ptr<VertexBuffer> m_vbo;
		std::shared_ptr<VertexBufferLayout> m_layout;
		std::shared_ptr<VertexArray> m_vao;
		std::shared_ptr<ElementBuffer> m_ebo;
		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<Shader> m_mainShader;
		// --- Knowledge about state of Renderer
		std::vector<std::string> _names;
		float _nextTextureID;
		bool m_pushedLayout = false;		// we need to push layout once, for every shape it is the same pattern
		bool m_initialized = false;		// check, if renderer is initialized
		bool m_isGUIconnected = false;	// check, which type of shader we want to use (we don't need gui calculations if it is not connected)
		RendererStatistics m_stats;

	public:
		Renderer() = default;

		void createRenderer(const std::shared_ptr<RendererFactory>& factory);
		void closeRenderer();

		void initialize(Mesh* mesh);
		void loadScene(Mesh* mesh, Scene* scene);

		void draw(Mesh* mesh);
		
		void connectGUI();
		void disconnectGUI();
		void updateGUIData(Mesh* mesh, const GUIData* guidata);
		void updateCameraData(const CameraData* cameradata);

		const std::string& getObjectName(const unsigned int& index) { return _names[index]; }
		const RendererStatistics& getStatistics() const { return m_stats; }

	private:
		void addShape(Mesh* mesh, std::shared_ptr<Shape>& shape, const glm::vec3& center, const glm::vec3& angle = {0.0f, 0.0f, 0.0f}, std::string texturePath = "empty");

		void bind();
		void unbind();
	};
}

#endif // !RENDERER_H