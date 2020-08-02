/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "mar_includes.h"

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

	namespace settings {
		inline const char name[10] = "MAREngine";
		inline const int width{ 1700 };
		inline const int height{ 900 };
		inline const char portName[10] = "\\\\.\\COM7";
		inline const char* glsl_version = "#version 330";
	}

	namespace constants {
		inline const size_t maxCount = 100000;
		inline const size_t maxVertexCount = maxCount * 50;
		inline const size_t maxIndexCount = maxCount * 50;
		inline const size_t maxObjectsInScene = 32;
	}

#endif // !MAR_ENGINE_SETTINGS
}