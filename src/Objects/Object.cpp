/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Object.h"
#include "../Debug/Log.h"

namespace mar {
	namespace graphics { namespace objects {


        void Object::assignDataFromFile(const std::string& path) {
            ObjectLoader::loadObject(path);

            m_name = ObjectLoader::getName();
            m_vertices = ObjectLoader::getVertices();
            m_indices = ObjectLoader::getIndices();
            m_layout = ObjectLoader::getLayout();
            m_center = ObjectLoader::getCenter();
            m_angle = ObjectLoader::getAngle();
            m_id = ObjectLoader::getID();
            m_texid = ObjectLoader::getTextureID();

            MAR_CORE_INFO("Assigned object from ObjectLoader!");
        }


} } }