/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_COMPONENTS_COLLECTIONS_H
#define MAR_ENGINE_ECS_COMPONENTS_COLLECTIONS_H

#include "../../../mar.h"


namespace mar {
	namespace ecs {


		struct CollectionTagComponent {
			std::string tag{ "EmptyCollection" };

			CollectionTagComponent() = default;
			CollectionTagComponent(std::string t)
				: tag(std::move(t))
			{}

			operator std::string& () { return tag; }
			operator const std::string& () const { return tag; }
		};


} }


#endif // !MAR_ENGINE_ECS_COMPONENTS_COLLECTIONS_H
