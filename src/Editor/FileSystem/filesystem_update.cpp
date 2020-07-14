/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "FileSystem.h"

namespace mar {
	namespace editor {


		std::vector<std::string> filesystem::s_marfiles;
		std::vector<std::string> filesystem::s_texturefiles;
	

		void filesystem::updateMarFiles(const char* path) {
			s_marfiles.clear();

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				std::string s = entry.path().filename().string();
				s_marfiles.push_back(s);
			}
		}

		void filesystem::updateMarTextures(const char* path) {
			s_texturefiles.clear();

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				std::string s = entry.path().filename().string();
				s_texturefiles.push_back(s);
			}
		}


} }
