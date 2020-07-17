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

		void ShapeManipulator::calculateVertexNormals(Ref<Shape>& shape) {
			std::vector<float> new_vertices_with_normals = calculateVertexNormals(shape->getVertices(), shape->getIndices(), shape->getStride());

			shape->setVertices(new_vertices_with_normals);
		}

		std::vector<float> ShapeManipulator::calculateVertexNormals(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, 
			const uint32_t& stride) {

			uint32_t size = vertices.size();
			std::vector<float> returnValue = vertices;
		
			maths::vec3 vec[3];
			maths::vec3 edge[2];
			maths::vec3 normal;

			for (uint32_t j = 0; j < size / stride; j++) {
				returnValue[j * stride + 3] = 0.f;
				returnValue[j * stride + 4] = 0.f;
				returnValue[j * stride + 5] = 0.f;
			}

			for (uint32_t i = 0; i < indices.size(); i += 3) {
				vec[0] = {
					vertices[ indices[i + 0] * stride + 0 ],
					vertices[ indices[i + 0] * stride + 1 ],
					vertices[ indices[i + 0] * stride + 2 ]
				};

				vec[1] = {
					vertices[ indices[i + 1] * stride + 0 ],
					vertices[ indices[i + 1] * stride + 1 ],
					vertices[ indices[i + 1] * stride + 2 ]
				};

				vec[2] = {
					vertices[ indices[i + 2] * stride + 0 ],
					vertices[ indices[i + 2] * stride + 1 ],
					vertices[ indices[i + 2] * stride + 2 ]
				};

				edge[0] = vec[0] - vec[1];
				edge[1] = vec[0] - vec[2];

				normal = edge[0].cross(edge[1]);

				returnValue[indices[i + 0] * stride + 0] += normal.x;
				returnValue[indices[i + 0] * stride + 1] += normal.y;
				returnValue[indices[i + 0] * stride + 2] += normal.x;
														 
				returnValue[indices[i + 1] * stride + 0] += normal.x;
				returnValue[indices[i + 1] * stride + 1] += normal.y;
				returnValue[indices[i + 1] * stride + 2] += normal.x;
														 
				returnValue[indices[i + 2] * stride + 0] += normal.x;
				returnValue[indices[i + 2] * stride + 1] += normal.y;
				returnValue[indices[i + 2] * stride + 2] += normal.x;
			}


			for (uint32_t j = 0; j < size / stride; j++) {
				float le = maths::vec3::length({ returnValue[j * stride + 3] ,
						returnValue[j * stride + 3] ,returnValue[j * stride + 3] });

				returnValue[j * stride + 3] /= le;
				returnValue[j * stride + 4] /= le;
				returnValue[j * stride + 5] /= le;
			}

			return returnValue;
		}


} }