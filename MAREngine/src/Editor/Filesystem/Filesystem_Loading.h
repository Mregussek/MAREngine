/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_EDITOR_FILESYSTEM_LOADING_H
#define MAR_ENGINE_EDITOR_FILESYSTEM_LOADING_H


#include "../../mar.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/Components.h"
#include "../../Core/ecs/ECS/EntityCollection.h"


namespace mar {
	namespace editor {


		class Filesystem_Loading {
		public:

			static void loadScene(std::ifstream& file, ecs::Scene* scene);
			static void loadEntity(std::ifstream& file, ecs::Scene* scene, ecs::Entity* entity);
			static void loadCollection(std::ifstream& file, ecs::Scene* scene, ecs::EntityCollection* collection);

		};


} }

#endif // !MAR_ENGINE_EDITOR_FILESYSTEM_LOADING_H