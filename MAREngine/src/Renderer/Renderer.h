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

	class Renderer {
		// --- Buffers
		std::shared_ptr<VertexBuffer> _vbo;
		std::shared_ptr<VertexBufferLayout> _lay;
		std::shared_ptr<VertexArray> _vao;
		std::shared_ptr<ElementBuffer> _ebo;
		std::shared_ptr<Texture> _texture;
		std::shared_ptr<Shader> _shader;
		// --- Objects
		std::vector<Shapes*> _shapes;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<int> _samplers;
		std::vector<glm::mat4> _translations;
		std::vector<glm::mat4> _rotations;
		// --- Helper object for creating new ones
		std::vector<Shapes*> _addedDuringRuntime;
		unsigned int _startupSceneSize;
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
		// --- Helpers
		bool _pushedOnce;       
		unsigned int _maxValue;     
		bool _initialized = false;
		// --- Statistics
		unsigned int _countOfDrawCalls;
		unsigned int _countOfVertices;
		unsigned int _countOfIndices;
		unsigned int _countOfShapes;

	public:
		Renderer() = default;

		void createRenderer(const std::shared_ptr<RendererFactory>& factory);
		void closeRenderer();

		void initialize();

		void pushObject(Shapes* shape, glm::vec3& position, std::string texturePath = TexturePaths.blackTex);
		void popObject(const unsigned int& index);

		void bind();
		void unbind();

		void updateFrame(); 
		void draw();
		void clear();
		
		void setGUIvectors(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles);
		void setGUImatrices(const float* colors, const glm::mat4& translationMatrix, const glm::mat4& rotationMatrix);
		void setCameraMatrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
		void setCameraVectors(const glm::vec3& position);

		void guiPush(GUIPushType pushType, glm::vec3& position);
		
		const std::string& getObjectName(unsigned int index);

		const std::vector<int>& getSamplers() const;
		const std::vector<unsigned int> getStatistics() const;

		const unsigned int& getSceneStartupSize() const;
	};
}

#endif // !RENDERER_H