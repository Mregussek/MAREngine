/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShapeManipulator.h"

namespace mar {
    namespace graphics {


		void ShapeManipulator::extendShapeID(std::vector<float>& vertices, const uint32_t& stride, const float& newid) {
			for (uint32_t i = 1; i < vertices.size() / stride + 1; i++) {
				vertices[i * stride - 1] = newid;
			}
		}

		void ShapeManipulator::extendTextureID(std::vector<float>& vertices, const uint32_t& stride, const float& newid) {
			for (uint32_t i = 1; i < vertices.size() / stride + 1; i++) {
				vertices[i * stride - 2] = newid;
			}
		}

		void ShapeManipulator::extendBothIDs(std::vector<float>& vertices, const uint32_t& stride, const float& newid) {
			for (uint32_t i = 1; i < vertices.size() / stride + 1; i++) {
				vertices[i * stride - 1] = newid;
				vertices[i * stride - 2] = newid;
			}
		}

		void ShapeManipulator::extendIndices(std::vector<uint32_t>& indices, const uint32_t& extension) {
			for (uint32_t i = 0; i < indices.size(); i++)
				indices[i] += extension;
		}

		std::vector<uint32_t> ShapeManipulator::changeIndicesFormat(const uint32_t& size, int& max_value,
			const std::vector<uint32_t>& passedValue) {

			std::vector<uint32_t> returnValue(passedValue.size());

			for (uint32_t i = 0; i < size; i++)
				returnValue[i] = passedValue[i] + max_value;

			return returnValue;
		}


} }