/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Log.h"

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

			s_file = PointerFactory<spdlog::sinks::basic_file_sink_mt>::makeRef("logs/log.txt");

			s_CoreLogger = PointerFactory<spdlog::logger>::makeRef("MAR ENGINE", s_file);
			s_CoreLogger->set_level(spdlog::level::trace);

			s_ClientLogger = PointerFactory<spdlog::logger>::makeRef("APPLICATION", s_file);
			s_ClientLogger->set_level(spdlog::level::trace);

			s_initialized = true;
		}


} }