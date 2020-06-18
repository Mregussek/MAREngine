/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FILE_SYSTEM_H 
#define MAR_ENGINE_FILE_SYSTEM_H

#include "../mar.h"
#include "../Debug/Log.h"


namespace mar {
	namespace filesystem {


		class FileSystem {
		public:
			void getTexturePaths(const std::filesystem::path& path) {
				for (const auto& p : std::filesystem::recursive_directory_iterator(path)) {
					if (!std::filesystem::is_directory(p)) {
						std::cout << p.path() << '\n';
					}
				}
			}
		};


} }


#endif // !MAR_ENGINE_FILE_SYSTEM_H
