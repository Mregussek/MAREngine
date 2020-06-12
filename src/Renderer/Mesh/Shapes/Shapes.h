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
		public:
			Shape() = default;

			Shape(float new_id, std::string new_name, glm::vec3 new_center, glm::vec3 new_angle,
				std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
				std::vector<unsigned int> new_layout)
				: m_id(new_id),
				m_texid(0.0f),
				m_name(new_name),
				m_center(new_center),
				m_angle(new_angle),
				m_verticesVector(new_vertices),
				m_indicesVector(new_indices),
				m_layout(new_layout),
				m_calculatedStride(false),
				m_stride(0)
			{}

			Shape(const Ref<Shape>& shape)
				: m_id(shape->getID()),
				m_texid(0.0f),
				m_name(shape->getName()),
				m_center(shape->getCenter()),
				m_angle(shape->getAngle()),
				m_verticesVector(shape->getVerticesVector()),
				m_indicesVector(shape->getIndicesVector()),
				m_layout(shape->getLayoutVector()),
				m_calculatedStride(false),
				m_stride(0)
			{}

			void assignDataFromFile(const std::string& path);

			virtual const std::string& getName() const;
			virtual void setName(const std::string& new_name);
			
			virtual void setID(float newID);
			virtual const float getID() const;
				 
			virtual void setTextureID(float newID);
			virtual const float getTextureID() const;
				
			virtual glm::vec3& getCenter();
			virtual void setCenter(const glm::vec3& new_center);
				
			virtual glm::vec3& getAngle();
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
			virtual void setLayout(const std::vector<unsigned int>& new_layout);
			virtual unsigned int getLayout(size_t index) const;
			virtual unsigned int getLayoutSize() const;

		private:
			float m_id;
			float m_texid;
			std::string m_name;

			glm::vec3 m_center;
			glm::vec3 m_angle;

			std::vector<float> m_verticesVector;
			std::vector<unsigned int> m_indicesVector;
			std::vector<unsigned int> m_layout;

			bool m_calculatedStride;
			unsigned int m_stride;
		};


} }

#endif // !SHAPES_H