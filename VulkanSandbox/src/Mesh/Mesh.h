

#ifndef MAR_ENGINE_GRAPHICS_MESH_H
#define MAR_ENGINE_GRAPHICS_MESH_H


#include "../../VulkanInclude.h"


namespace mar {


	class Mesh {
	public:

		std::string m_name;

		std::vector<objl::Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		void loadFromFile(const char* path);

	};


}


#endif // !MAR_ENGINE_GRAPHICS_MESH_H
