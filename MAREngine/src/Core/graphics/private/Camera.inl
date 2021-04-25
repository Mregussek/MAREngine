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


#include "../public/Camera.h"


namespace marengine {


    template<typename TCamera> bool FCameraKeyboardDecorator::update(TCamera* pCamera, int32 key) {
        return false;
    }

    template<typename TCamera> bool FCameraMouseDecorator::update(TCamera* pCamera, int32 key) {
        return false;
    }

    template<typename TCamera> bool FCameraSphericalDecorator::update(TCamera* pCamera, int32 key) {
        return false;
    }


    template<> bool FCameraKeyboardDecorator::update<FCamera3D>(FCamera3D* pCamera, int32 key);
    template<> bool FCameraMouseDecorator::update<FCamera3D>(FCamera3D* pCamera, int32 key);
    template<> bool FCameraSphericalDecorator::update<FCamera3D>(FCamera3D* pCamera, int32 key);


}
