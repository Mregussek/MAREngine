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


#ifndef MARENGINE_PYTHONINTERPRETER_H
#define MARENGINE_PYTHONINTERPRETER_H


#include <string>


namespace marengine {


    class FPythonInterpreter {
    public:

        static void init();

        static std::string changeSlashesToDots(std::string script);

        static std::string getModuleFromPath(std::string script);

    };


}



#endif //MARENGINE_PYTHONINTERPRETER_H
