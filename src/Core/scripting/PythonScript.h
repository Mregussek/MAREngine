/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_SCRIPTING_EMBED_H
#define MAR_ENGINE_SCRIPTING_EMBED_H


#include "../../mar.h"


namespace mar {
    namespace ecs { class Entity; }

	namespace scripting {
        namespace py = pybind11;
        

        class PythonScript {
            py::module scriptModule;
            py::object module;
            bool initialized;

        public:
            PythonScript()
                : initialized(false)
            {}

            void loadScript(const char* from, const char* what);

            void start(const ecs::Entity& e);

            void update(ecs::Entity& e);

            py::object& getModule() { return module; }

        private:
            void appendCurrentPath() {
                auto os = py::module::import("os");
                auto path = os.attr("path").attr("abspath")(os.attr("getcwd")());

                auto sys = py::module::import("sys");
                sys.attr("path").attr("insert")(0, path);
            }
        };


} }


#endif // !MAR_ENGINE_SCRIPTING_EMBED_H