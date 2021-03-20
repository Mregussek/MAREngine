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


#ifndef MARENGINE_ISERVICEMANAGER_H
#define MARENGINE_ISERVICEMANAGER_H


namespace marengine {


    template<typename TDerivedServiceManager, typename TServiceType>
    class IServiceManager {
    public:

        virtual void emplace(TServiceType* service) {
            static_cast<TDerivedServiceManager*>(this)->emplace(service);
        }

        virtual void pop(TServiceType* service) {
            static_cast<TDerivedServiceManager*>(this)->pop(service);
        }

        virtual void onCreate() const {
            static_cast<const TDerivedServiceManager*>(this)->onCreate();
        }

        virtual void onUpdate() const {
            static_cast<const TDerivedServiceManager*>(this)->onUpdate();
        }

        virtual void onDestroy() const {
            static_cast<const TDerivedServiceManager*>(this)->onDestroy();
        }

    };


}


#endif //MARENGINE_ISERVICEMANAGER_H
