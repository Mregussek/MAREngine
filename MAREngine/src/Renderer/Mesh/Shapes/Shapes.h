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

	class Shapes {
	public:
		Shapes(float new_id, glm::vec3 new_center, glm::vec3 new_angle,
			std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
			std::vector<unsigned int> new_layout)
				: id(new_id),
				center(new_center),
				angle(new_angle),
				verticesVector(new_vertices),
				indicesVector(new_indices),
				layout(new_layout),
				_calculatedStride(false),
				_stride(0)
		{}

		virtual void setID(float newID);
		virtual const float getID() const;

		virtual const glm::vec3 getCenter() const;
		virtual void setCenter(const glm::vec3& new_center);

		virtual const glm::vec3 getAngle() const;
		virtual void setAngle(const glm::vec3& new_angle);

		virtual const unsigned int getStride();
		virtual const unsigned int getSizeofVertices() const;
		virtual const unsigned int getSizeofIndices() const;
		virtual const unsigned int getMaxValueOfIndices() const;

		virtual void setVerticesVector(const std::vector<float>& new_vertices);
		virtual const std::vector<float>& getVerticesVector() const;
		virtual float getVertice(size_t index) const;
		virtual void setVertice(size_t index, float new_value);
		virtual std::vector<float>::const_iterator getVerticesBegin() const;
		virtual std::vector<float>::const_iterator getVerticesEnd() const;

		virtual void setIndicesVector(const std::vector<unsigned int>& new_indices);
		virtual const std::vector<unsigned int>& getIndicesVector() const;
		virtual unsigned int getIndice(size_t index) const;
		virtual void setIndice(size_t index, unsigned int new_value);
		virtual std::vector<unsigned int>::const_iterator getIndicesBegin() const;
		virtual std::vector<unsigned int>::const_iterator getIndicesEnd() const;

		virtual unsigned int getLayout(size_t index) const;
		virtual unsigned int getLayoutSize() const;

	private:
		float id;
		glm::vec3 center;
		glm::vec3 angle;

		std::vector<float> verticesVector;

		std::vector<unsigned int> indicesVector;
		std::vector<unsigned int> layout;

		bool _calculatedStride;
		unsigned int _stride;
	};
}

#endif // !SHAPES_H