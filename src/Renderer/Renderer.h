/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"
#include "RendererFactory.h"

namespace mar {

	namespace constants {
		const size_t maxCount = 1000; 
		const size_t maxVertexCount = maxCount * 10 * sizeof(Vertex);
		const size_t maxIndexCount = maxCount * 5;
		const size_t maxObjectsInScene = 32;
	}

	enum class GUIPushType {
		CUBE,
		WALL,
		SURFACE,
		PYRAMID
	};

	struct RendererStatistics {
		unsigned int _countOfDrawCalls;
		unsigned int _countOfVertices;
		unsigned int _countOfIndices;
		unsigned int _countOfShapes;

		RendererStatistics()
			:_countOfDrawCalls(0),
			_countOfVertices(0),
			_countOfIndices(0),
			_countOfShapes(0)
		{}
	};

	class Renderer {
		// --- Buffers
		std::shared_ptr<VertexBuffer> _vbo;
		std::shared_ptr<VertexBufferLayout> _lay;
		std::shared_ptr<VertexArray> _vao;
		std::shared_ptr<ElementBuffer> _ebo;
		std::shared_ptr<Texture> _texture;
		std::shared_ptr<Shader> _mainShader;
		// --- Objects
		std::shared_ptr<std::vector<std::shared_ptr<Shapes>>> _shapes;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<int> _samplers;
		std::vector<glm::mat4> _translations;
		std::vector<glm::mat4> _rotations;
		// --- Helper object for creating new ones
		std::shared_ptr<Shapes> _addedDuringRuntime;
		// --- Lightning
		glm::vec3 _lightPosition{ 0.0f, 0.5f, 5.0f };
		// --- Setup
		const float* _gui_colors;
		glm::mat4 _gui_rotation;
		glm::mat4 _gui_translate;
		glm::mat4 _camera_model;
		glm::mat4 _camera_projection;
		glm::mat4 _camera_view;
		glm::vec3 _camera_position;
		// --- Knowledge about state of Renderer
		bool _pushedLayout = false;		// we need to push layout once, for every shape it is the same pattern
		unsigned int _maxValue;			// there is need to know max value of indices in order to push more object properly
		bool _initialized = false;		// check, if renderer is initialized
		bool _isGUIconnected = false;	// check, which type of shader we want to use (we don't need gui calculations if it is not connected)
		// --- Statistics
		RendererStatistics _stats;

	public:
		Renderer() = default;

		void createRenderer(const std::shared_ptr<RendererFactory>& factory);
		void closeRenderer();

		void initialize();

		void loadScene(Scene* scene);

		void pushObject(std::shared_ptr<Shapes>& shape, glm::vec3& position, std::string texturePath = TexturePaths.blackTex);

		void popObject(const unsigned int& index);

		void bind();
		void unbind();

		void updateFrame(); 
		void draw();
		void clearScreen();
		
		void setGUIvectors(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles);
		void setGUImatrices(const float* colors, const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix);
		void setCameraMatrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
		void setCameraVectors(const glm::vec3& position);

		void guiPush(GUIPushType pushType, glm::vec3& position);
		
		const std::string& getObjectName(unsigned int index);

		const RendererStatistics& getStatistics() const;

		const std::vector<int>& getSamplers() const;

		void connectGUI() { _isGUIconnected = true; }
		void disconnectGUI() { _isGUIconnected = false; }
	};
}

#endif // !RENDERER_H