/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "mar_includes.h"
#include "mar_logging.h"

/*!
	mar is a main namespace for MAREngine
	If class is here declared, it means that there is linker problem for other files with it,
	so I decided to put it here.
*/
namespace mar {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

#ifndef MAR_ENGINE_SETTINGS
#define MAR_ENGINE_SETTINGS

	template<typename T>
	struct PointerFactory {
		inline static uint32_t ref_counter;
		inline static uint32_t scope_counter;

		template<typename... Args>
		static std::unique_ptr<T> makeScope(Args&&... args) {
			ref_counter++;
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename... Args>
		static std::shared_ptr<T> makeRef(Args&&... args) {
			scope_counter++;
			return std::make_shared<T>(std::forward<Args>(args)...);
		}
	};

#endif // !MAR_ENGINE_SETTINGS
}