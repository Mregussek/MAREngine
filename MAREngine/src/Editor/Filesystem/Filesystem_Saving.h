/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H
#define MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H


#include "../../mar.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/Components.h"
#include "../../Core/ecs/ECS/EntityCollection.h"


namespace mar {
	namespace editor {


		class Filesystem_Saving {
		public:
			static void saveScene(std::ofstream& ss, ecs::Scene* scene);
			static void saveCollection(std::ofstream& ss, const ecs::EntityCollection& collection);
			static void saveEntity(std::ofstream& ss, const ecs::Entity& entity);
		};


} }


#endif // !MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H
