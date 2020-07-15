/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "mar.h"


namespace mar {


	template<typename T>
	unsigned int PointerFactory<T>::ref_counter{ 0 };

	template<typename T>
	unsigned int PointerFactory<T>::scope_counter{ 0 };


}