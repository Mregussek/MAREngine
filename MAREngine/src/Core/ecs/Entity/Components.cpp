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


#include "Components.h"


namespace marengine {


    bool CRenderable::MaterialInfo::isValid() const {
        return index != -1 && type != EMaterialType::NONE && !path.empty() && path != "None";
    }

	maths::mat4 CTransform::getTransform() const {
		maths::mat4 tran;
		tran.recompose(position, maths::quat(rotation), scale);
		return tran;
	}

    bool CCamera::isMainCamera() const {
        return id.find("main") != std::string::npos;
    }


}
