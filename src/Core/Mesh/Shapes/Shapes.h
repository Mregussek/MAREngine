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
			std::string m_name;
			std::string m_usedTexture;
			std::string m_usedOBJ;

			float m_generalScale;

			maths::vec3 m_center;
			maths::vec3 m_angle;
			maths::vec3 m_scaleVec;
			maths::vec3 m_defaultColor;

			std::vector<float> m_verticesVector;
			std::vector<uint32_t> m_indicesVector;
			std::vector<uint32_t> m_layout;

			bool m_calculatedStride;
			uint32_t m_stride;

		public:
			Shape() = default;

			Shape(float new_id, const char* new_name, maths::vec3 new_center, maths::vec3 new_angle,
				std::vector<float> new_vertices, std::vector<uint32_t> new_indices,
				std::vector<uint32_t> new_layout, maths::vec3 new_color);

			Shape(const Ref<Shape>& shape);

			void assignDataFromFile(const char* path);

			// --- GET METHODS --- //
			inline const std::string& getName() const { return m_name; }
			inline const std::string& getUsedTexture() const { return m_usedTexture; }
			inline const std::string& getUsedObj() const { return m_usedOBJ; }
			inline const float getID() const { return m_id; }
			inline const float getTextureID() const { return m_texid; }
			inline float& getGeneralScale() { return m_generalScale; }
			inline maths::vec3& getCenter() { return m_center; }
			inline maths::vec3& getAngle() { return m_angle; }
			inline maths::vec3& getScaleVec() { return m_scaleVec; }
			inline maths::vec3& getDefaultColor() { return m_defaultColor; }
			const uint32_t getStride();
			inline const std::vector<float>& getVertices() const { return m_verticesVector; }
			inline const std::vector<uint32_t>& getIndices() const { return m_indicesVector; }
			inline const std::vector<uint32_t>& getLayout() const { return m_layout; }

			// --- SET METHODS --- //
			void setName(std::string new_name) { 
				m_name = new_name; 
				MAR_CORE_TRACE("SHAPE: set new name");
			}
			void setUsedTexture(std::string used_tex) { 
				m_usedTexture = used_tex; 
				MAR_CORE_TRACE("SHAPE: set new used texture");
			}
			void setUsedObj(std::string used_obj) { 
				m_usedOBJ = used_obj; 
				MAR_CORE_TRACE("SHAPE: set new .obj file");
			}
			void setID(float new_id) { 
				m_id = new_id; 
				MAR_CORE_TRACE("SHAPE: set new ID");
			}
			void setTextureID(float new_tex_id) { 
				m_texid = new_tex_id; 
				MAR_CORE_TRACE("SHAPE: set new texture ID");
			}
			void setGeneralScale(float new_general) {
				m_generalScale = new_general;
				MAR_CORE_TRACE("SHAPE: set general scale");
			}
			void setCenter(const maths::vec3& new_center) { 
				m_center = new_center; 
				MAR_CORE_TRACE("SHAPE: set new center");
			}
			void setAngle(const maths::vec3& new_angle) { 
				m_angle = new_angle; 
				MAR_CORE_TRACE("SHAPE: set new angle");
			}
			void setScaleVec(const maths::vec3& new_scale) { 
				m_scaleVec = new_scale; 
				MAR_CORE_TRACE("SHAPE: set new scale vector");
			}
			void setDefaultColor(const maths::vec3& new_color) { 
				m_defaultColor = new_color; 
				MAR_CORE_TRACE("SHAPE: set new default color");
			}
			void setVertices(const std::vector<float>& new_vertices) {
				m_verticesVector = new_vertices; 
				MAR_CORE_TRACE("SHAPE: set new vertices");
			}
			void setVertice(size_t index, float new_value) { 
				m_verticesVector[index] = new_value; 
				MAR_CORE_TRACE("SHAPE: set new vertice at index");
			}
			void setIndices(const std::vector<uint32_t>& new_indices) { 
				m_indicesVector = new_indices; 
				MAR_CORE_TRACE("SHAPE: set new indices");
			}
			void setIndice(size_t index, uint32_t new_value) { 
				m_indicesVector[index] = new_value; 
				MAR_CORE_TRACE("SHAPE: set new indice at index");
			}
			void setLayout(const std::vector<uint32_t>& new_layout) { 
				m_layout = new_layout; 
				MAR_CORE_TRACE("SHAPE: set new layout");
			}
		};


} }

#endif // !SHAPES_H