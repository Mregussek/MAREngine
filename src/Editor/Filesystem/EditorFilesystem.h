/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_FILESYSTEM_H
#define MAR_ENGINE_EDITOR_FILESYSTEM_H

#include "../../mar.h"
#include "../../Core/Scene/Scene.h"
#include "../../Core/Scene/Entity/Entity.h"
#include "../../Core/Scene/Component/Components.h"


namespace mar {
	namespace editor {


		class Filesystem {
		public:

			static void saveToFile(ecs::Scene* scene, const char* filename);
			static ecs::Scene* openFile(const char* filename);

		};


} }


#endif // !MAR_ENGINE_EDITOR_FILESYSTEM_H
