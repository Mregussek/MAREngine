/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShapeManipulator.h"

namespace mar {
    namespace graphics {


		void ShapeManipulator::extendShapeID(Ref<Shape>& shape, const float& nextID) {
			uint32_t size = shape->getVertices().size();
			uint32_t stride = shape->getStride();

			// extend all vertices, which defines texture m_id
			for (uint32_t j = 1; j < size / stride + 1; j++)
				shape->setVertice(j * stride - 1, nextID);

			shape->setID(nextID);
		}

		void ShapeManipulator::extendTextureID(Ref<Shape>& shape, const float& nextID) {
			uint32_t size = shape->getVertices().size();
			uint32_t stride = shape->getStride();

			// extend all vertices, which defines texture m_id
			for (uint32_t j = 1; j < size / stride + 1; j++)
				shape->setVertice(j * stride - 2, nextID);

			shape->setTextureID(nextID);
		}

		void ShapeManipulator::changeIndicesFormat(Ref<Shape>& shape, int& max_value) {
			if (max_value == 0) return;

			std::vector<uint32_t> new_indices = changeIndicesFormat(shape->getIndices().size(), max_value, shape->getIndices());
			shape->setIndices(new_indices);
		}

		std::vector<uint32_t> ShapeManipulator::changeIndicesFormat(const uint32_t& size, int& max_value,
			const std::vector<uint32_t>& passedValue) {

			std::vector<uint32_t> returnValue(passedValue.size());

			for (uint32_t i = 0; i < size; i++)
				returnValue[i] = passedValue[i] + max_value;

			return returnValue;
		}


} }