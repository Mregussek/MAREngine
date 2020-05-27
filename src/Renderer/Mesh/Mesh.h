/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_H
#define MAR_ENGINE_MESH_H

#include "../../mar.h"
#include "../Renderer.h"

namespace mar {

    class Mesh {
		std::shared_ptr<Texture> _texture;
		std::vector<int> _samplers;
		// --- Objects
		std::vector<std::shared_ptr<Shape>> _shapes;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		// --- Helper object for creating new ones
		std::shared_ptr<Shape> _addedDuringRuntime;
		// --- Lightning
		glm::vec3 _lightPosition{ 0.0f, 0.5f, 5.0f };
		// --- Knowledge about state of Renderer
		float _nextShapeID;
		float _nextTextureID;
		bool _pushedLayout = false;		// we need to push layout once, for every shape it is the same pattern
		unsigned int _maxValue;			// there is need to know max value of indices in order to push more object properly

	public:
		Mesh() = default;

		void initialize();
		void createMesh(Scene* scene);
		void deleteMesh();
    
		void pushObject(std::shared_ptr<Shape>& shape, glm::vec3& position, std::string texturePath = "empty");
		void popObject(const unsigned int& index);

		void bind();
		void unbind();
    
		void insertVertices(const std::shared_ptr<Shape>& shape);
		void insertIndices(const std::shared_ptr<Shape>& shape);
	};

}

#endif // !MAR_ENGINE_MESH_H

