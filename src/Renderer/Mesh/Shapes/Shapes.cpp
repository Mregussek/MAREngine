/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shapes.h"

namespace mar {
	namespace graphics {


		Shape::Shape(float new_id, const char* new_name, maths::vec3 new_center, maths::vec3 new_angle,
			std::vector<float> new_vertices, std::vector<unsigned int> new_indices,
			std::vector<unsigned int> new_layout, maths::vec3 new_color)
			: m_id(new_id),
			m_texid(0.0f),
			m_name(new_name),
			m_center(new_center),
			m_angle(new_angle),
			m_scale({ 1.f, 1.f, 1.f }),
			m_defaultColor(new_color),
			m_verticesVector(new_vertices),
			m_indicesVector(new_indices),
			m_layout(new_layout),
			m_calculatedStride(false),
			m_stride(0)
		{}

		Shape::Shape(const Ref<Shape>& shape)
			: m_id(shape->getID()),
			m_texid(0.0f),
			m_name(shape->getName()),
			m_center(shape->getCenter()),
			m_angle(shape->getAngle()),
			m_scale(shape->getScale()),
			m_defaultColor(shape->getDefaultColor()),
			m_verticesVector(shape->getVertices()),
			m_indicesVector(shape->getIndices()),
			m_layout(shape->getLayout()),
			m_calculatedStride(false),
			m_stride(0)
		{}

		void Shape::assignDataFromFile(const char* path) {
			objects::ObjectLoader::loadObject(path);

			m_name = objects::ObjectLoader::getName();
			m_verticesVector = objects::ObjectLoader::getVertices();
			m_indicesVector = objects::ObjectLoader::getIndices();
			m_layout = objects::ObjectLoader::getLayout();
			m_center = objects::ObjectLoader::getCenter();
			m_angle = objects::ObjectLoader::getAngle();
			m_id = objects::ObjectLoader::getID();
			m_texid = (float)objects::ObjectLoader::getTextureID();
			m_scale = objects::ObjectLoader::getScale();
			m_defaultColor = objects::ObjectLoader::getColor();
			m_usedOBJ = path;

			MAR_CORE_INFO("Assigned object from ObjectLoader!");
		}

		const unsigned int Shape::getStride() {
			if (!m_calculatedStride) {
				for (auto const& l : m_layout) m_stride += l;
				m_calculatedStride = true;
				return m_stride;
			}

			return m_stride;
		}


} }