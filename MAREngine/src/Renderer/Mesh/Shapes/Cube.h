/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef CUBE_H
#define CUBE_H

#include "../../../mar.h"
#include "Shapes.h"

namespace mar {

	class Cube : public Shapes, std::enable_shared_from_this<Cube> {
	public:
		Cube() = default;

		// --- ID Get / Set --- //
		void setID(float newID) override;
		const float getID() const override;

		// --- ID Get / Set --- //
		void setCenter(const glm::vec3& new_center) override;
		const glm::vec3 getCenter() const override;

		// --- ID Get / Set --- //
		const glm::vec3 getAngle() const { return angle; }
		void setAngle(const glm::vec3& new_angle) { angle = new_angle; }
		
		// --- Vertices methods --- //
		void setVerticesVector(const std::vector<float>& new_vertices) override;
		const std::vector<float>& getVerticesVector() const override;
		float getVertice(size_t index) const override;
		void setVertice(size_t index, float new_value) override;
		std::vector<float>::const_iterator getVerticesBegin() const override;
		std::vector<float>::const_iterator getVetricesEnd() const override;
		const unsigned int getSizeofVertices() const override;

		// --- Indices Methods --- //
		void setIndicesVector(const std::vector<unsigned int>& new_indices) override;
		const std::vector<unsigned int>& getIndicesVector() const override;
		unsigned int getIndice(size_t index) const override;
		void setIndice(size_t index, unsigned int new_value) override;
		std::vector<unsigned int>::const_iterator getIndicesBegin() const override;
		std::vector<unsigned int>::const_iterator getIndicesEnd() const override;
		const unsigned int getSizeofIndices() const override;
		const unsigned int getMaxValueOfIndices() const override;

		// --- Layout Methods --- //
		unsigned int getLayout(size_t index) const override;
		unsigned int getLayoutSize() const override;
		const unsigned int getStride() override;

	private:
		float id = 0.0f;
		glm::vec3 center{ 0.0f, 0.0f, 0.0f };
		glm::vec3 angle{ 0.0f, 0.0f, 0.0f };

		std::vector<float> verticesVector = {
			//  front (x, y, z)		// Texture		// Texture Index
			-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f,
			 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f,
			 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,		0.0f,
			-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f,
			//  back 								
			-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f,
			 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f,
			 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f,
			-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f
		};

		std::vector<unsigned int> indicesVector = {
			// front	// back
			0, 1, 2,	7, 6, 5,
			2, 3, 0,	5, 4, 7,
			// right	// left
			1, 5, 6,	4, 0, 3,
			6, 2, 1,	3, 7, 4,
			// bottom	// top
			4, 5, 1,	3, 2, 6,
			1, 0, 4,	6, 7, 3
		};

		std::vector<unsigned int> layout = {
			3, 2, 1
		};

		bool _calculatedStride = false; // we need to calculate stride once, then it is "known" value
		unsigned int _stride = 0;
	};

}

#endif // !CUBE_H