/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "Log.h"


#ifdef MAR_ENGINE_DEBUG_MODE


namespace marengine {

	bool Log::s_initialized{ false };
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;


	void Log::init() {
		if (s_initialized) {
			return;
		}

		spdlog::set_pattern("%^[%T] %n: %v%$");

		constexpr bool terminalLogging{ true };

		if constexpr (terminalLogging) {
			s_CoreLogger = spdlog::stdout_color_mt("MAR ENGINE");
		}
		else {
			struct tm ltm;
			time_t now = time(0);
			localtime_s(&ltm, &now);
			std::string filename = // we got format logs/log<year><month><day>_<hour><min><sec>.txt Ex. logs/log20200730_23546.txt
				"logs/log" +
				std::to_string(1900 + ltm.tm_year) + std::to_string(1 + ltm.tm_mon) + std::to_string(ltm.tm_mday) +
				"_" +
				std::to_string(ltm.tm_hour) + std::to_string(1 + ltm.tm_min) + std::to_string(1 + ltm.tm_sec) +
				".txt";

			auto max_size = 1048576 * 35;
			auto max_files = 10;

			s_CoreLogger = spdlog::rotating_logger_mt("MAR ENGINE", filename, max_size, max_files);
		}

		s_CoreLogger->set_level(spdlog::level::warn);

		s_initialized = true;
	}


}


#endif