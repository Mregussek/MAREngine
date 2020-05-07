/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"
#include "RendererFactory.h"
#include "Mesh/Mesh.h"

namespace mar {

	namespace constants {
		const size_t maxCubeCount = 1000;
		const size_t maxVertexCount = maxCubeCount * 16;
		const size_t maxIndexCount = maxCubeCount * 24;
	}

	class Renderer {
		// --- Buffers
		std::shared_ptr<VertexBuffer> _vbo;
		std::shared_ptr<VertexBufferLayout> _lay;
		std::shared_ptr<VertexArray> _vao;
		std::shared_ptr<ElementBuffer> _ebo;
		std::shared_ptr<Texture> _texture;
		std::shared_ptr<Mesh> _mesh;
		// --- Objects
		std::vector<Shapes*> _shapes;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<int> _samplers;
		// --- Helpers
		bool _pushedOnce;       
		unsigned int _maxValue;     
		bool _initialized = false;

	public:
		Renderer() = default;
		~Renderer();

		void initializeRenderer(const std::shared_ptr<RendererFactory>& factory);

		void initializeBuffers();

		void pushObject(Shapes* shape, glm::vec3& position, std::string& texturePath);

		void popObject();

		void bind();
		void unbind();

		void updateFrame(const std::vector<glm::vec3>& newCenters, const std::vector<glm::vec3>& newAngles);
		
		void draw() const;
		void clear();

		const std::vector<int>& getSamplers() const { return _samplers; }
	};
}

#endif // !RENDERER_H