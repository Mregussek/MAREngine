

#include "Mesh.h"


namespace mar {


	void Mesh::loadFromFile(const char* path) {
        objl::Loader Loader{};
        const bool loadout = Loader.LoadFile(path);

        if (loadout) {
            m_vertices = Loader.LoadedVertices;
            m_indices = Loader.LoadedIndices;
        }
        else {
           std::cout << "MESH_CREATOR: could not load .obj file " << path << "\n";
        }
	}


}
