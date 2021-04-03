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


#ifndef MARENGINE_MAR_INL
#define MARENGINE_MAR_INL


namespace marengine {


    template<typename TBase, typename TDerived>
    inline TBase* staticCast(TDerived* pDerived) {
        return static_cast<TBase*>(pDerived);
    }

    template<typename TBase, typename TDerived>
    inline const TBase* staticCast(const TDerived* pDerived) {
        return static_cast<const TBase*>(pDerived);
    }

    template<typename TBase, typename TDerived>
    inline TBase* dynamicCast(TDerived* pDerived) {
        if constexpr (std::is_convertible<TDerived*, TBase*>()::value) {
            return static_cast<TBase*>(pDerived);
        }
        else if constexpr (std::is_base_of<TBase, TDerived>()::value) {
            return static_cast<TBase*>(pDerived);
        }
        
        return nullptr;
    }

    template<typename TBase, typename TDerived>
    inline const TBase* dynamicCast(const TDerived* pDerived) {
        if constexpr (std::is_convertible<const TDerived*, const TBase*>()::value) {
            return static_cast<const TBase*>(pDerived);
        }
        else if constexpr (std::is_base_of<TBase, TDerived>()::value) {
            return static_cast<const TBase*>(pDerived);
        }
        
        return nullptr;
    }


}


#endif //MARENGINE_MAR_INL