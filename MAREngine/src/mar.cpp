/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "mar.h"


namespace mar {


	template<typename T>
	uint32_t PointerFactory<T>::ref_counter{ 0 };

	template<typename T>
	uint32_t PointerFactory<T>::scope_counter{ 0 };


}