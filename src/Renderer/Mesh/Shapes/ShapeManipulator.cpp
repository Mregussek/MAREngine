/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShapeManipulator.h"

namespace mar {
    namespace graphics {


		void ShapeManipulator::extendShapeID(Ref<Shape>& shape, const float& nextID) {
			if (nextID == 0.0f) return;

			unsigned int size = shape->getSizeofVertices();
			unsigned int stride = shape->getStride();

			// extend all vertices, which defines texture m_id
			for (unsigned int j = 1; j < size / stride + 1; j++)
				shape->setVertice(j * stride - 1, nextID);

			shape->setID(nextID);
		}

		void ShapeManipulator::extendTextureID(Ref<Shape>& shape, const float& nextID) {
			if (nextID == 0.0f) {
				shape->setTextureID(nextID);
				return;
			}

			unsigned int size = shape->getSizeofVertices();
			unsigned int stride = shape->getStride();

			// extend all vertices, which defines texture m_id
			for (unsigned int j = 1; j < size / stride + 1; j++)
				shape->setVertice(j * stride - 2, nextID);

			shape->setTextureID(nextID);
		}

		void ShapeManipulator::changeIndicesFormat(Ref<Shape>& shape, int& max_value) {
			if (max_value == 0) return;

			std::vector<unsigned int> new_indices = changeIndicesFormat(shape->getSizeofIndices(), max_value, shape->getIndicesVector());
			shape->setIndicesVector(new_indices);
		}

		std::vector<unsigned int> ShapeManipulator::changeIndicesFormat(const unsigned int& size, int& max_value,
			const std::vector<unsigned int>& passedValue) {

			std::vector<unsigned int> returnValue(passedValue.size());

			for (unsigned int i = 0; i < size; i++)
				returnValue[i] = passedValue[i] + max_value;

			return returnValue;
		}


} }