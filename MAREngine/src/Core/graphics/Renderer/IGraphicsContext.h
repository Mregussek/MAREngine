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


#ifndef MARENGINE_IGRAPHICSCONTEXT_H
#define MARENGINE_IGRAPHICSCONTEXT_H


namespace marengine {

    class FWindow;


    template<typename TDerivedContext>
    class IGraphicsContext {
    public:

        bool create(FWindow* pWindow) {
            return static_cast<TDerivedContext*>(this)->create(pWindow);
        }

        void close() {
            static_cast<TDerivedContext*>(this)->close();
        }

        void prepareFrame() {
            static_cast<TDerivedContext*>(this)->prepareFrame();
        }

        void endFrame() {
            static_cast<TDerivedContext*>(this)->endFrame();
        }

    };

    template<typename TLibraryDependentDerivedContext>
    class FGraphicsContext : public IGraphicsContext<FGraphicsContext<TLibraryDependentDerivedContext>> {
    public:

        bool create(FWindow* pWindow) {
            return static_cast<TLibraryDependentDerivedContext*>(this)->create(pWindow);
        }

        void close() {
            static_cast<TLibraryDependentDerivedContext*>(this)->close();
        }

        void prepareFrame() {
            static_cast<TLibraryDependentDerivedContext*>(this)->prepareFrame();
        }

        void endFrame() {
            static_cast<TLibraryDependentDerivedContext*>(this)->endFrame();
        }

    };


}


#endif //MARENGINE_IGRAPHICSCONTEXT_H
