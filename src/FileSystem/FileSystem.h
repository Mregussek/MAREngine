/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FILE_SYSTEM_H 
#define MAR_ENGINE_FILE_SYSTEM_H

#include "../mar.h"
#include "../Debug/Log.h"
#include "../Layers/MeshLayer.h"


namespace mar {
	namespace filesystem {


		void saveSceneToFile(const char* path, const std::vector<graphics::Mesh*>& meshes);
		std::vector<layers::MeshLayer*> loadSceneFromFile(const char* path);


} }


#endif // !MAR_ENGINE_FILE_SYSTEM_H
