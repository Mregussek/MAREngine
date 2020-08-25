/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Log.h"

#ifdef MAR_ENGINE_DEBUG_MODE

namespace mar {
	namespace debug {

		bool Log::s_initialized{ false };
		Ref<spdlog::logger> Log::s_CoreLogger;

		void Log::init() {
			if (s_initialized) {
				return;
			}

			spdlog::set_pattern("%^[%T] %n: %v%$");

#ifndef MAR_ENGINE_TERMINAL_LOGS
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
#else
			s_CoreLogger = spdlog::stdout_color_mt("MAR ENGINE");
#endif

#if defined(MAR_ENGINE_LOGS_LEVEL_TRACE)
			s_CoreLogger->set_level(spdlog::level::trace);
#elif defined(MAR_ENGINE_LOGS_LEVEL_INFO)
			s_CoreLogger->set_level(spdlog::level::info);
#elif defined(MAR_ENGINE_LOGS_LEVEL_WARN)
			s_CoreLogger->set_level(spdlog::level::warn);
#elif defined(MAR_ENGINE_LOGS_LEVEL_ERROR)
			s_CoreLogger->set_level(spdlog::level::err);
#endif

			s_initialized = true;
		}


} }


#endif