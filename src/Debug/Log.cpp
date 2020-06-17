/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Log.h"

namespace mar {
	namespace debug {

		bool Log::s_initialized{ false };
		Ref<spdlog::logger> Log::s_CoreLogger;
		Ref<spdlog::logger> Log::s_ClientLogger;

		void Log::init() {
			if (s_initialized) {
				return;
			}

			spdlog::set_pattern("%^[%T] %n: %v%$");

			s_CoreLogger = spdlog::stdout_color_mt("MAR ENGINE");
			s_CoreLogger->set_level(spdlog::level::trace);

			s_ClientLogger = spdlog::stderr_color_mt("APPLICATION");
			s_ClientLogger->set_level(spdlog::level::trace);

			s_initialized = true;
		}


} }