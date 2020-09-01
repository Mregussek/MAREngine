/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShapeManipulator.h"

namespace mar {
    namespace graphics {


		void ShapeManipulator::extendShapeID(std::vector<float>& vertices, uint32_t stride, float newid) {
			for (uint32_t i = 1; i < vertices.size() / stride + 1; i++) 
				vertices[i * stride - 1] = newid;
			
			GRAPHICS_TRACE("SHAPE_MANIPULATOR: extendShapeID(vert.size() = {}, stride = {}, newid = {})", vertices.size(), stride, newid);
		}

		void ShapeManipulator::extendIndices(std::vector<uint32_t>& indices, uint32_t extension) {
			for (uint32_t i = 0; i < indices.size(); i++)
				indices[i] += extension;
		}

		void ShapeManipulator::extendIndices(std::vector<uint32_t>& indices, uint32_t start, uint32_t end, uint32_t extension) {
			for (uint32_t i = start; i < end; i++)
				indices[i] += extension;

			GRAPHICS_TRACE("SHAPE_MANIPULATOR: extendIndices(indices.size() = {}, start = {}, end = {}, extension = {})", indices.size(), start, end, extension);
		}

		void ShapeManipulator::calculateNormals(std::vector<float>& vertices, const std::vector<uint32_t>& indices, const int32_t stride) {
			if (indices.size() % 3 != 0)
				GRAPHICS_ERROR("SHAPEMANIPULATOR: indices.size() is not divisible by 3!!!");

			GRAPHICS_TRACE("SHAPE_MANIPULATOR: going to calculate vertex normals");

			int32_t index[3];
			for (size_t i = 0; i < indices.size(); i += 3) {
				maths::vec3 triangles[3];

				for (int32_t j = 0; j < 3; j++) {
					index[j] = indices[i + j] * stride;
					triangles[j].x = vertices[index[j] + 0];
					triangles[j].y = vertices[index[j] + 1];
					triangles[j].z = vertices[index[j] + 2];
				}

				maths::vec3 ab = triangles[1] - triangles[0];
				maths::vec3 ac = triangles[2] - triangles[0];

				maths::vec3 cr = maths::vec3::cross(ab, ac);
				cr = maths::vec3::normalize(cr);

				for (int32_t j = 0; j < 3; j++) {
					vertices[index[j] + 3] += cr.x;
					vertices[index[j] + 4] += cr.y;
					vertices[index[j] + 5] += cr.z;
				}
			}

			GRAPHICS_INFO("SHAPE_MANIPULATOR: calculated vertex normals");
		}


} }