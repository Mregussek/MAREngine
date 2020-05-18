/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "../../../mar.h"

namespace mar {

	//! Vertex
	/*!
		Vertex is a structure, which holds all the information about format of data.
		For VBO!
		Now it is not implemented, but in future (with .obj files loading) there is going
		to be need for some refactoring.
	*/
	struct Vertex {
		float position[3];
		float basicColors[3];
		float lightNormal[3];
		float texCoord[2];
		float texId;
	};

	//! Shapes
	/*!
		Shapes is a base class for the whole types of possible shapes.
		It defines structure and encapsulation of private data, which
		cannot be accesed anywhere.

		I think for now there is no need to document every method here,
		because names of functions says all what they do.
	*/
	class Shape {
	public:
		Shape(float new_id, std::string new_name, glm::vec3 new_center, glm::vec3 new_angle,
			std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
			std::vector<unsigned int> new_layout)
				: id(new_id),
				name(new_name),
				center(new_center),
				angle(new_angle),
				verticesVector(new_vertices),
				indicesVector(new_indices),
				layout(new_layout),
				_calculatedStride(false),
				_stride(0)
		{}

		Shape(const std::shared_ptr<Shape>& shape)
			: id(shape->getID()),
			name(shape->getName()),
			center(shape->getCenter()),
			angle(shape->getAngle()),
			verticesVector(shape->getVerticesVector()),
			indicesVector(shape->getIndicesVector()),
			layout(shape->getLayoutVector()),
			_calculatedStride(false),
			_stride(0)
		{}

		virtual const std::string& getName() const;

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

		virtual const std::vector<unsigned int>& getLayoutVector() const;
		virtual unsigned int getLayout(size_t index) const;
		virtual unsigned int getLayoutSize() const;

	private:
		float id;
		std::string name;

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