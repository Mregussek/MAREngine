/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "../../../mar.h"
#include "../Objects/ObjectLoader.h"
#include "../../../Debug/Log.h"

namespace mar {
	namespace graphics {


		//! Vertex
		/*!
			Vertex is a structure, which holds all the information about format of data.
			For VBO!
			Now it is not implemented, but in future (with .obj files loading) there is going
			to be need for some Refactoring.
		*/
		struct Vertex {
			float position[3];
			float basicColors[3];
			float lightNormal[3];
			float texCoord[2];
			float texId;
			float shapeId;
		};

		//! Shape
		/*!
			Shape is a base class for the whole types of possible shapes.
			It defines structure and encapsulation of private data, which
			cannot be accessed anywhere.

			I think for now there is no need to document every method here,
			because names of functions says all what they do.
		*/
		class Shape {
			float m_id;
			float m_texid;
			std::string m_name{};

			glm::vec3 m_center;
			glm::vec3 m_angle;
			glm::vec3 m_defaultColor;

			std::vector<float> m_verticesVector;
			std::vector<unsigned int> m_indicesVector;
			std::vector<unsigned int> m_layout;

			bool m_calculatedStride;
			unsigned int m_stride;

		public:
			Shape() = default;

			Shape(float new_id, std::string new_name, glm::vec3 new_center, glm::vec3 new_angle,
				std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
				std::vector<unsigned int> new_layout, glm::vec3 new_color);

			Shape(const Ref<Shape>& shape);

			void assignDataFromFile(const char* path);

			// --- GET METHODS --- //
			const std::string& getName() const;
			const float getID() const;
			const float getTextureID() const;
			glm::vec3& getCenter();
			glm::vec3& getAngle();
			glm::vec3& getDefaultColor();
			const unsigned int getStride();
			const unsigned int getSizeofVertices() const;
			const unsigned int getSizeofIndices() const;
			const unsigned int& getMaxValueOfIndices() const;
			const unsigned int& getMinValueOfIndices() const;
			const std::vector<float>& getVerticesVector() const;
			float getVertice(size_t index) const;
			std::vector<float>::const_iterator getVerticesBegin() const;
			std::vector<float>::const_iterator getVerticesEnd() const;
			const std::vector<unsigned int>& getIndicesVector() const;
			unsigned int getIndice(size_t index) const;
			std::vector<unsigned int>::const_iterator getIndicesBegin() const;
			std::vector<unsigned int>::const_iterator getIndicesEnd() const;
			const std::vector<unsigned int>& getLayoutVector() const;
			unsigned int getLayout(size_t index) const;
			unsigned int getLayoutSize() const;

			// --- SET METHODS --- //
			void setName(const std::string& new_name);
			void setID(float newID);
			void setTextureID(float newID);
			void setCenter(const glm::vec3& new_center);
			void setAngle(const glm::vec3& new_angle);
			void setDefaultColor(const glm::vec3& new_color);
			void setVerticesVector(const std::vector<float>& new_vertices);
			void setVertice(size_t index, float new_value);
			void setIndicesVector(const std::vector<unsigned int>& new_indices);
			void setIndice(size_t index, unsigned int new_value);
			void setLayout(const std::vector<unsigned int>& new_layout);
			
		};


} }

#endif // !SHAPES_H