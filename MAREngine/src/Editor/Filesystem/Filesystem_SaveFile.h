/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H
#define MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H


#include "../../mar.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/ComponentsEntity.h"
#include "../../Core/ecs/ECS/EntityCollection.h"
#include "../../Core/ecs/ECS/ComponentsCollection.h"


namespace mar {
	namespace editor {


		class Filesystem_SaveFile {
		public:
			static void saveCollection(std::ofstream& ss, const ecs::EntityCollection& collection);
			static void saveEntity(std::ofstream& ss, const ecs::Entity& entity);
		};


} }


#endif // !MAR_ENGINE_EDITOR_FILESYSTEM_SAVE_TO_FILE_H
