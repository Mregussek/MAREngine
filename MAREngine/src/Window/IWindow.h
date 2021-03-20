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


#ifndef MARENGINE_IWINDOW_H
#define MARENGINE_IWINDOW_H


#include "../mar.h"
#include "buttons_def.h"


namespace marengine {


    /**
     * @class IWindow IWindow.h "Window/IWindow.h"
     * @brief IWindow is a interface class for Window in MAREngine. Every derived implementation of IWindow should
     * properly implement those methods for proper run.
     */
    class IWindow {
    public:

        /**
         * @brief Method is responsible for opening window with given width, height and its name.
         * @param width width of window
         * @param height height of window
         * @param name name of window, that shall be displayed
         * @return True if window opened correctly, false otherwise
         */
        virtual bool open(uint32_t width, uint32_t height, const char* name) = 0;

        /// @brief Closes current window instance. Remember, it does not terminate library, closes window!
        virtual void close() = 0;

        /**
         * @brief Initializes implementation library of window instance. It is called during MAREngine's startup.
         * @return True if window's library initialized correctly, false otherwise
         */
        virtual bool initializeLibrary() = 0;

         /// @brief Terminates window's library. Called during MAREngine's termination.
        virtual void terminateLibrary() = 0;

        /**
         * @brief Initializes GUI Editor Library with window instance, so that it can be used correctly.
         * It is called during editor gui initialization.
         */
        virtual void initEditorGuiLibrary() = 0;

        /**
         * @brief Begins new frame at editor GUI library. Sometimes these methods need to have access to window
         * instance, so that it should be implemented there. Called during every new frame.
         */
        virtual void beginNewFrameEditorGuiLibrary() = 0;

         /// @brief Terminates Editor GUI library, so that window afterwards can be closed correctly.
        virtual void terminateEditorGuiLibrary() = 0;

        /**
         * @brief Checks if window is going to close after that frame and returns the result.
         * @return True, if window is going to close, false otherwise
         */
        MAR_NO_DISCARD virtual bool isGoingToClose() = 0;

        /**
         * @brief Sets vertical synchronization (decreases FPS if set on!).
         * @param vsSetValue vsSetValue equal to 1 sets VS. 0 disables it.
         */
        virtual void setVerticalSync(int32_t vsSetValue) = 0;

        /// @brief Swaps buffers and polls event.
        virtual void swapBuffers() = 0;

        /**
         * @brief Returns window size on x-axis.
         * @return window size on x-axis.
         */
        MAR_NO_DISCARD virtual int32_t getSizeX() const = 0;

        /**
         * @brief Returns window size on y-axis.
         * @return window size on y-axis.
         */
        MAR_NO_DISCARD virtual int32_t getSizeY() const = 0;

        /**
         * @brief Checks, if given key at keyboard was pressed and returns result. For correct key
         * value check Window/buttons_def.h file.
         * @param key Keyboard value, to be checked if it was pressed.
         * @return True, if keyboard key was pressed, false otherwise.
         */
        MAR_NO_DISCARD virtual bool isKeyPressed(int32_t key) const = 0;

        /**
         * @brief Checks, if given key at mouse was pressed and returns result. For correct key
         * value check Window/buttons_def.h file.
         * @param key Mouse value, to be checked if it was pressed.
         * @return True, if mouse key was pressed, false otherwise.
         */
        MAR_NO_DISCARD virtual bool isMousePressed(int32_t key) const = 0;

        /**
         * @brief Returns Mouse position at x-axis.
         * @return Mouse position at x-axis.
         */
        MAR_NO_DISCARD virtual float getMousePositionX() const = 0;

        /**
         * @brief Returns Mouse position at y-axis.
         * @return Mouse position at y-axis.
         */
        MAR_NO_DISCARD virtual float getMousePositionY() const = 0;

        /**
         * @brief Returns scroll value at x-axis.
         * @return scroll value at x-axis.
         */
        MAR_NO_DISCARD virtual float getScrollX() const = 0;

        /**
         * @brief Returns scroll value at y-axis.
         * @return scroll value at y-axis.
         */
        MAR_NO_DISCARD virtual float getScrollY() const = 0;

    };


    /**
     * @struct FWindowCurrentInfo IWindow.h "Window/IWindow.h"
     * @brief FWindowCurrentInfo is a structure storage for all current information about window. All implementations
     * of IWindow should contain this structure and fill it during runtime.
     */
    struct FWindowCurrentInfo {

        uint32_t width{ 800 };
        uint32_t height{ 600 };
        float xScroll{ 0.f };
        float yScroll{ 0.f };
        float xMousePos{ 0.f };
        float yMousePos{ 0.f };

    };


}


#endif //MARENGINE_IWINDOW_H
