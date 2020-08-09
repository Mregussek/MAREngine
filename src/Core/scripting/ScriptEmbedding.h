/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_SCRIPTING_EMBED_H
#define MAR_ENGINE_SCRIPTING_EMBED_H


#include "../../mar.h"



namespace mar {
	namespace scripting {
        namespace py = pybind11;
        

        class ScriptEmbedding {
            py::module scriptModule;
            py::object module;
            bool initialized;

        public:
            ScriptEmbedding()
                : initialized(false)
            {}

            void loadScript(const char* from, const char* what) {
                auto os = py::module::import("os");
                auto path = os.attr("path").attr("abspath")(os.attr("getcwd")());

                auto sys = py::module::import("sys");
                sys.attr("path").attr("insert")(0, path);

                scriptModule = py::module::import(from);
                module = scriptModule.attr(what)();

                initialized = true;
            }

            void start() {
                if (!initialized)
                    return;

                module.attr("start")();
            }

            void update() {
                if (!initialized)
                    return;

                module.attr("update")();
            }

            py::object& getModule() { return module; }

        };


} }


#endif // !MAR_ENGINE_SCRIPTING_EMBED_H