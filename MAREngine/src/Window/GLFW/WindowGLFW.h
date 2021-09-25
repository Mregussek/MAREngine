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


#ifndef MARENGINE_FWINDOWGLFW_H
#define MARENGINE_FWINDOWGLFW_H


#include "../Window.h"


namespace marengine {


    /**
     * @class FWindowGLFW WindowGLFW.h "Window/GLFW/WindowGLFW.h"
     * @brief FWindowGLFW class is second implementation of IWindow. It is basic implementation of
     * GLFW library and gives basic functions.
     */
	class FWindowGLFW : public FWindow {
	public:

        /**
         * @brief Method is responsible for opening window with given width, height and its name.
         * @param width width of window
         * @param height height of window
         * @param name name of window, that shall be displayed
         * @return True if window opened correctly, false otherwise
         */
	    bool open(uint32_t width, uint32_t height, const char* name) final;

        /// @brief Closes current window instance. Remember, it does not terminate library,
        /// closes window!
	    void close() final;

        /**
         * @brief Initializes implementation library of window instance. It is called during
         * MAREngine's startup.
         * @return True if window's library initialized correctly, false otherwise
         */
	    bool initializeLibrary() final;

        /// @brief Terminates window's library. Called during MAREngine's termination.
	    void terminateLibrary() final;

        /**
         * @brief Checks if window is going to close after that frame and returns the result.
         * @return True, if window is going to close, false otherwise
         */
	    MAR_NO_DISCARD bool isGoingToClose() final;

        /**
         * @brief Sets vertical synchronization (decreases FPS if set on!).
         * @param vsSetValue vsSetValue equal to 1 sets VS. 0 disables it.
         */
	    void setVerticalSync(int32_t vsSetValue) final;

        /// @brief Clears screen
	    void clear() const final;

        /// @brief Swaps buffers and polls event.
	    void swapBuffers() final;

        /**
         * @brief Checks, if given key at keyboard was pressed and returns result. For correct key
         * value check Window/buttons_def.h file.
         * @param key Keyboard value, to be checked if it was pressed.
         * @return True, if keyboard key was pressed, false otherwise.
         */
        MAR_NO_DISCARD bool isKeyPressed(int32_t key) const final;

        /**
         * @brief Checks, if given key at mouse was pressed and returns result. For correct key
         * value check Window/buttons_def.h file.
         * @param key Mouse value, to be checked if it was pressed.
         * @return True, if mouse key was pressed, false otherwise.
         */
        MAR_NO_DISCARD bool isMousePressed(int32_t key) const final;

	protected:

        GLFWwindow* p_pWindowContext{ nullptr };

	};


    /**
     * @class FWindowGLFWImGui WindowGLFW.h "Window/GLFW/WindowGLFW.h"
     * @brief FWindowGLFWImGui class is third implementation of IWindow. It derives from FWindowGLFW
     * implementation and updates mandatory methods for editor.
     */
	class FWindowGLFWImGui : public FWindowGLFW {
	public:

        /**
         * @brief Initializes GUI Editor Library with window instance, so that it can be used
         * correctly. It is called during editor gui initialization.
         */
        void initEditorGuiLibrary() final;

        /// @brief Begins new frame at editor GUI library. Called during every new frame.
        void beginNewFrameEditorGuiLibrary() final;

        /// @brief Terminates EditorGUI library, so that window afterwards can be closed correctly.
        void terminateEditorGuiLibrary() final;

    };


}


#endif // !MARENGINE_FWINDOWGLFW_H
