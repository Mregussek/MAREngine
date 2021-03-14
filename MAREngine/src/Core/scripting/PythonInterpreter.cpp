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


#include "PythonInterpreter.h"
#include "../../ProjectManager.h"
#include "../../mar.h"


namespace marengine {


    void FPythonInterpreter::init() {
        static pybind11::scoped_interpreter guard{}; // we can have only existing instance

        auto os = pybind11::module::import("os");
        auto path = os.attr("path").attr("abspath")(os.attr("getcwd")());

        auto sys = pybind11::module::import("sys");
        sys.attr("path").attr("insert")(0, path);
    }

    std::string FPythonInterpreter::changeSlashesToDots(std::string script) {
        const auto& assetsPath = FProjectManager::getAssetsPath();
        std::string rtn = assetsPath + script;

        size_t pos = rtn.find("/");

        while (pos != std::string::npos) {
            rtn.replace(pos, 1, ".");
            pos = rtn.find("/", pos + 1);
        }

        rtn = rtn.substr(0, rtn.size() - 3);

        return rtn;
    }

    std::string FPythonInterpreter::getModuleFromPath(std::string script) {
        const std::string filename = script.substr(script.find_last_of("/") + 1, script.size());
        const std::string deletedDotPyFromPath = filename.substr(0, filename.size() - 3);

        return deletedDotPyFromPath;
    }


}