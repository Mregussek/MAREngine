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
			const char* m_name;
			std::string m_usedTexture;
			std::string m_usedOBJ;

			maths::vec3 m_center;
			maths::vec3 m_angle;
			maths::vec3 m_scaleVec;
			maths::vec3 m_defaultColor;

			maths::mat4 m_translation;
			maths::mat4 m_rotation;
			maths::mat4 m_scaleMat;

			std::vector<float> m_verticesVector;
			std::vector<unsigned int> m_indicesVector;
			std::vector<unsigned int> m_layout;

			bool m_calculatedStride;
			unsigned int m_stride;

		public:
			Shape() = default;

			Shape(float new_id, const char* new_name, maths::vec3 new_center, maths::vec3 new_angle,
				std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
				std::vector<unsigned int> new_layout, maths::vec3 new_color);

			Shape(const Ref<Shape>& shape);

			void assignDataFromFile(const char* path);

			// --- GET METHODS --- //
			inline const char* getName() const { return m_name; }
			inline const std::string& getUsedTexture() const { return m_usedTexture; }
			inline const std::string& getUsedObj() const { return m_usedOBJ; }
			inline const float getID() const { return m_id; }
			inline const float getTextureID() const { return m_texid; }
			inline maths::vec3& getCenter() { return m_center; }
			inline maths::vec3& getAngle() { return m_angle; }
			inline maths::vec3& getScaleVec() { return m_scaleVec; }
			inline maths::vec3& getDefaultColor() { return m_defaultColor; }
			inline maths::mat4& getTranslation() { return m_translation; }
			inline maths::mat4& getRotation() { return m_rotation; }
			inline maths::mat4& getScaleMat() { return m_scaleMat; }
			const unsigned int getStride();
			inline const std::vector<float>& getVertices() const { return m_verticesVector; }
			inline const std::vector<unsigned int>& getIndices() const { return m_indicesVector; }
			inline const std::vector<unsigned int>& getLayout() const { return m_layout; }

			// --- SET METHODS --- //
			void setName(const char* new_name) { m_name = new_name; }
			void setUsedTexture(std::string used_tex) { m_usedTexture = used_tex; }
			void setUsedObj(std::string used_obj) { m_usedOBJ = used_obj; }
			void setID(float new_id) { m_id = new_id; }
			void setTextureID(float new_tex_id) { m_texid = new_tex_id; }
			void setCenter(const maths::vec3& new_center) { m_center = new_center; }
			void setAngle(const maths::vec3& new_angle) { m_angle = new_angle; }
			void setScaleVec(const maths::vec3& new_scale) { m_scaleVec = new_scale; }
			void setTranslation(const maths::mat4& new_trans) { m_translation = new_trans; }
			void setRotation(const maths::mat4& new_rot) { m_rotation = new_rot; }
			void setScaleMat(const maths::mat4& new_scal) { m_scaleMat = new_scal; }
			void setDefaultColor(const maths::vec3& new_color) { m_defaultColor = new_color; }
			void setVertices(const std::vector<float>& new_vertices) { m_verticesVector = new_vertices; }
			void setVertice(size_t index, float new_value) { m_verticesVector[index] = new_value; }
			void setIndices(const std::vector<unsigned int>& new_indices) { m_indicesVector = new_indices; }
			void setIndice(size_t index, unsigned int new_value) { m_indicesVector[index] = new_value; }
			void setLayout(const std::vector<unsigned int>& new_layout) { m_layout = new_layout; }
		};


} }

#endif // !SHAPES_H