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


#ifndef MAR_ENGINE_SCRIPTING_COMPONENTS_H
#define MAR_ENGINE_SCRIPTING_COMPONENTS_H


#include "../../../mar.h"
#include "../../scripting/PythonScript.h"


namespace marengine {


	/*
		PythonScriptComponent - used for working with python scripting in MAREngine.
		Contains PythonScript instance, for loading module and updating entity during execution.
		Also there is scriptsPath member, which should contain correct path to script (ended with .py extension for file)
		and sourceCode with which we can modify current module and save it file after modification.
	*/
	struct PythonScriptComponent {

		PythonScript pythonScript;
		std::string scriptsPath{ "empty" };
		std::string sourceCode{ "empty" };


		PythonScriptComponent() = default;
		PythonScriptComponent(const PythonScriptComponent & sc) = default;
		PythonScriptComponent(std::string s);

	};


}


#endif // !MAR_ENGINE_ECS_COMPONENTS_SCRIPTING_COMPONENTS_H
