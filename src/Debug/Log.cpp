/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Log.h"

#ifdef MAR_ENGINE_DEBUG_MODE

namespace mar {
	namespace debug {

		bool Log::s_initialized{ false };
		Ref<spdlog::sinks::basic_file_sink_mt> Log::s_file;
		Ref<spdlog::logger> Log::s_CoreLogger;
		Ref<spdlog::logger> Log::s_ClientLogger;


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

			s_file = PointerFactory<spdlog::sinks::basic_file_sink_mt>::makeRef(filename);
			s_CoreLogger = PointerFactory<spdlog::logger>::makeRef("MAR ENGINE", s_file);
#else
			s_CoreLogger = spdlog::stdout_color_mt("MAR ENGINE");
#endif
			s_CoreLogger->set_level(spdlog::level::trace);

			//s_ClientLogger = PointerFactory<spdlog::logger>::makeRef("APPLICATION", s_file);
			//s_ClientLogger->set_level(spdlog::level::trace);

			s_initialized = true;
		}


} }


#endif