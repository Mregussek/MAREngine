/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAR_ENGINE_SCRIPTING_PYTHON_SCRIPT_H
#define MAR_ENGINE_SCRIPTING_PYTHON_SCRIPT_H


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
            PythonScript();

            void loadScript(const char* from, const char* what);

            void start(const ecs::Entity* e);

            void update(ecs::Entity* e);

            py::object& getModule() { return module; }

            static void appendCurrentPath();
        };


} }


#endif // !MAR_ENGINE_SCRIPTING_PYTHON_SCRIPT_H