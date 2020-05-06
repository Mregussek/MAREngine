/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "../../../mar.h"

namespace mar {

	struct Vertex {
		float position[3];
		float texCoord[2];
		float texId;
	};

	class Shapes : std::enable_shared_from_this<Shapes> {
	public:
		virtual void setID(float newID) = 0;
		virtual const float getID() const = 0;

		virtual const glm::vec3 getCenter() const = 0;
		virtual void setCenter(const glm::vec3& new_center) = 0;

		virtual const glm::vec3 getAngle() const = 0;
		virtual void setAngle(const glm::vec3& new_angle) = 0;

		virtual const unsigned int getStride() = 0;
		virtual const unsigned int getSizeofVertices() const = 0;
		virtual const unsigned int getSizeofIndices() const = 0;
		virtual const unsigned int getMaxValueOfIndices() const = 0;

		virtual void setVerticesVector(const std::vector<float>& new_vertices) = 0;
		virtual const std::vector<float>& getVerticesVector() const = 0;
		virtual float getVertice(size_t index) const = 0;
		virtual void setVertice(size_t index, float new_value) = 0;
		virtual std::vector<float>::const_iterator getVerticesBegin() const = 0;
		virtual std::vector<float>::const_iterator getVetricesEnd() const = 0;

		virtual void setIndicesVector(const std::vector<unsigned int>& new_indices) = 0;
		virtual const std::vector<unsigned int>& getIndicesVector() const = 0;
		virtual unsigned int getIndice(size_t index) const = 0;
		virtual void setIndice(size_t index, unsigned int new_value) = 0;
		virtual std::vector<unsigned int>::const_iterator getIndicesBegin() const = 0;
		virtual std::vector<unsigned int>::const_iterator getIndicesEnd() const = 0;

		virtual unsigned int getLayout(size_t index) const = 0;
		virtual unsigned int getLayoutSize() const = 0;
	};
}

#endif // !SHAPES_H