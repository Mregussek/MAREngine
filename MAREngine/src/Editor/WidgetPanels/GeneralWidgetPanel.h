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


#ifndef MAR_ENGINE_W_GENERAL_WIDGET_PANEL_H
#define MAR_ENGINE_W_GENERAL_WIDGET_PANEL_H


#include "../../mar.h"
#include "IWidgetPanel.h"


namespace marengine {


	class WGeneralWidgetPanel : public IWidgetPanel {
	public:

		virtual void create() override;
		virtual void destroy() override;

		virtual void beginFrame() override;
		virtual void endFrame() override;

	private:

		bool m_dockspaceOpen{ true };
		static const bool s_fullscreenPersisant{ true };

	};


}


#endif // !MAR_ENGINE_W_GENERAL_WIDGET_PANEL_H