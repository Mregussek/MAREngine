/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_H
#define MAR_ENGINE_MESH_H

#include "../../mar.h"
#include "../Renderer.h"
#include "Light/Light.h"

namespace mar {

    class Mesh {
		std::vector<std::shared_ptr<Shape>> _shapes;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<int> _samplers;
		std::vector<glm::mat4> _translations;
		std::vector<glm::mat4> _rotations;
        Light _light;

		float _nextShapeID;
        unsigned int _maxValue;

    public:
        void createMesh();

        void pushShape(std::shared_ptr<Shape>& new_shape);
        void popShape(const unsigned int& index);
        
        void pushMatrices(const glm::vec3& center, const glm::vec3& angle);
        void popMatrices(const unsigned int& index);

        void clearBuffers();
        void clearMatrices();

        void update();





    public:
        const unsigned int& getShapesCount() const { return _shapes.size(); }
        const std::vector<float>& getVertices() const { return _vertices; }
        const std::vector<unsigned int>& getIndices() const { return _indices; }
        const std::vector<int>& getSamplers() const { return _samplers; }
        const std::vector<glm::mat4>& getTranslationMatrices() const { return _translations; }
        const std::vector<glm::mat4>& getRotationMatrices() const { return _rotations; }
        const int& getSamplerID(const unsigned int& index) { return _samplers[index]; }
        const unsigned int& getVerticesSize() const { return _vertices.size(); }
        const unsigned int& getIndicesSize() const { return _indices.size(); }
        const unsigned int& getSamplersSize() const { return _samplers.size(); }
        const std::vector<unsigned int>& getLayout() const;
        const unsigned int& getLayoutSize() const;
        const glm::vec3& getLightPosition() const { return _light.getPosition(); }
	};

}

#endif // !MAR_ENGINE_MESH_H

