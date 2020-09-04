/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "GUI_EntityCollectionPanel.h"

namespace mar {
	namespace editor {


		ecs::EntityCollection* GUI_EntityCollectionPanel::currentCollection{ nullptr };
		int32_t GUI_EntityCollectionPanel::currentIndex{ -1 };

} }
