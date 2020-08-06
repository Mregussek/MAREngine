/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_SCRIPTING_EMBED_H
#define MAR_ENGINE_SCRIPTING_EMBED_H

#if __has_include(<pybind11/pybind11.h>)
	#include <pybind11/pybind11.h>
	#include <pybind11/embed.h>
#else
	#error "MARMathPythonModule: Cannot import pybind11/pybind11.h!"
#endif



namespace mar {
	namespace scripting {

        namespace py = pybind11;


        class ScriptEmbedding {
            py::object module;
            bool initialized;

        public:
            ScriptEmbedding()
                : initialized(false)
            {}

            void loadScript(const char* from, const char* what) {
                module = py::module::import(from).attr(what);

                initialized = true;
            }

            void start() {
                if (!initialized)
                    return;

                module.attr("start")(module);
            }

            void update() {
                if (!initialized)
                    return;

                module.attr("update")(module);
            }

        };


} }


#endif // !MAR_ENGINE_SCRIPTING_EMBED_H