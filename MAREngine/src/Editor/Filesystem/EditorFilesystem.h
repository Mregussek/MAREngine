/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_FILESYSTEM_H
#define MAR_ENGINE_EDITOR_FILESYSTEM_H

#include "../../mar.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/ComponentsEntity.h"


namespace mar {
	namespace editor {


		class Filesystem {
		public:

			static void saveToFile(ecs::Scene* scene, const char* filename);
			static ecs::Scene* openFile(std::string filename);

			static std::string loadPyScript(const char* filename);
			static void savePyScript(const char* filename, std::string source);

		};


} }


#endif // !MAR_ENGINE_EDITOR_FILESYSTEM_H
