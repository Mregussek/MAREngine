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


#ifndef MAR_ENGINE_SCRIPT_IDE_FILESYSTEM_WIDGETS_H
#define MAR_ENGINE_SCRIPT_IDE_FILESYSTEM_WIDGETS_H


#include "../IWidgetPanel.h"
#include <string>


namespace marengine {


	class WScriptIDEFilesystemWidgets : public IWidgetPanel {
	public:

		static WScriptIDEFilesystemWidgets* Instance;

		virtual void create() override;
		virtual void updateFrame() override;

		void openCreateNewEditorScriptWidget() const;
		void openSaveAsEditorScriptWidget() const;
		void openOpenInEditorScriptWidget() const;

	private:

		const std::string createNewEditorScript{ "Create New Python Script On Text Editor" };
		const std::string saveAsEditorScript{ "Save As Python Script on Text Editor" };
		const std::string openInEditorScript{ "Open In Editor Python Script" };

		const std::string py{ ".py" };

	};


}


#endif // !MAR_ENGINE_SCRIPT_IDE_FILESYSTEM_WIDGETS_H
