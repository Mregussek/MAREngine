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


#ifndef MAR_ENGINE_W_ENTITY_FILESYSTEM_WIDGETS_H
#define MAR_ENGINE_W_ENTITY_FILESYSTEM_WIDGETS_H


#include "../../../mar.h"
#include "../IWidgetPanel.h"


namespace marengine {


	class WEntityFilesystemWidgets : public IWidgetPanel {
	public:

		static WEntityFilesystemWidgets* Instance;

		WEntityFilesystemWidgets() = default;

		virtual void create() override;

		virtual void updateFrame() override;


		void openLoadOBJWidget() const;
		void openCreateAndAssignPythonScriptWidget() const;
		void openAssignPythonScriptWidget() const;
		
	private:

		struct {
			const std::string loadObj{ "Load .obj for entity" };
			const std::string createAndAssignPythonScript{ "Create Python Script and assign it to Entity" };
			const std::string assignPythonScript{ "Assign Python Script to Entity" };
		} WidgetNames;
		
		struct {
			const std::string obj{ ".obj" };
			const std::string py{ ".py" };
		} FileExtensions;
		
	};


}



#endif // !MAR_ENGINE_W_ENTITY_FILESYSTEM_WIDGETS_H
