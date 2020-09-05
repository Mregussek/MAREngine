/**
 *				MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "../mar.h"


#define MAR_KEY_RELEASE               0
#define MAR_KEY_PRESS                 1
#define MAR_KEY_REPEAT                2

// --- KEYBOARD DEFINES --- //

/* The unknown key */
#define MAR_KEY_UNKNOWN            -1

/* Printable keys */
#define MAR_KEY_SPACE              32
#define MAR_KEY_APOSTROPHE         39  /* ' */
#define MAR_KEY_COMMA              44  /* , */
#define MAR_KEY_MINUS              45  /* - */
#define MAR_KEY_PERIOD             46  /* . */
#define MAR_KEY_SLASH              47  /* / */
#define MAR_KEY_0                  48
#define MAR_KEY_1                  49
#define MAR_KEY_2                  50
#define MAR_KEY_3                  51
#define MAR_KEY_4                  52
#define MAR_KEY_5                  53
#define MAR_KEY_6                  54
#define MAR_KEY_7                  55
#define MAR_KEY_8                  56
#define MAR_KEY_9                  57
#define MAR_KEY_SEMICOLON          59  /* ; */
#define MAR_KEY_EQUAL              61  /* = */
#define MAR_KEY_A                  65
#define MAR_KEY_B                  66
#define MAR_KEY_C                  67
#define MAR_KEY_D                  68
#define MAR_KEY_E                  69
#define MAR_KEY_F                  70
#define MAR_KEY_G                  71
#define MAR_KEY_H                  72
#define MAR_KEY_I                  73
#define MAR_KEY_J                  74
#define MAR_KEY_K                  75
#define MAR_KEY_L                  76
#define MAR_KEY_M                  77
#define MAR_KEY_N                  78
#define MAR_KEY_O                  79
#define MAR_KEY_P                  80
#define MAR_KEY_Q                  81
#define MAR_KEY_R                  82
#define MAR_KEY_S                  83
#define MAR_KEY_T                  84
#define MAR_KEY_U                  85
#define MAR_KEY_V                  86
#define MAR_KEY_W                  87
#define MAR_KEY_X                  88
#define MAR_KEY_Y                  89
#define MAR_KEY_Z                  90
#define MAR_KEY_LEFT_BRACKET       91  /* [ */
#define MAR_KEY_BACKSLASH          92  /* \ */
#define MAR_KEY_RIGHT_BRACKET      93  /* ] */
#define MAR_KEY_GRAVE_ACCENT       96  /* ` */
#define MAR_KEY_WORLD_1            161 /* non-US #1 */
#define MAR_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define MAR_KEY_ESCAPE             256
#define MAR_KEY_ENTER              257
#define MAR_KEY_TAB                258
#define MAR_KEY_BACKSPACE          259
#define MAR_KEY_INSERT             260
#define MAR_KEY_DELETE             261
#define MAR_KEY_RIGHT              262
#define MAR_KEY_LEFT               263
#define MAR_KEY_DOWN               264
#define MAR_KEY_UP                 265
#define MAR_KEY_PAGE_UP            266
#define MAR_KEY_PAGE_DOWN          267
#define MAR_KEY_HOME               268
#define MAR_KEY_END                269
#define MAR_KEY_CAPS_LOCK          280
#define MAR_KEY_SCROLL_LOCK        281
#define MAR_KEY_NUM_LOCK           282
#define MAR_KEY_PRINT_SCREEN       283
#define MAR_KEY_PAUSE              284
#define MAR_KEY_F1                 290
#define MAR_KEY_F2                 291
#define MAR_KEY_F3                 292
#define MAR_KEY_F4                 293
#define MAR_KEY_F5                 294
#define MAR_KEY_F6                 295
#define MAR_KEY_F7                 296
#define MAR_KEY_F8                 297
#define MAR_KEY_F9                 298
#define MAR_KEY_F10                299
#define MAR_KEY_F11                300
#define MAR_KEY_F12                301
#define MAR_KEY_F13                302
#define MAR_KEY_F14                303
#define MAR_KEY_F15                304
#define MAR_KEY_F16                305
#define MAR_KEY_F17                306
#define MAR_KEY_F18                307
#define MAR_KEY_F19                308
#define MAR_KEY_F20                309
#define MAR_KEY_F21                310
#define MAR_KEY_F22                311
#define MAR_KEY_F23                312
#define MAR_KEY_F24                313
#define MAR_KEY_F25                314
#define MAR_KEY_KP_0               320
#define MAR_KEY_KP_1               321
#define MAR_KEY_KP_2               322
#define MAR_KEY_KP_3               323
#define MAR_KEY_KP_4               324
#define MAR_KEY_KP_5               325
#define MAR_KEY_KP_6               326
#define MAR_KEY_KP_7               327
#define MAR_KEY_KP_8               328
#define MAR_KEY_KP_9               329
#define MAR_KEY_KP_DECIMAL         330
#define MAR_KEY_KP_DIVIDE          331
#define MAR_KEY_KP_MULTIPLY        332
#define MAR_KEY_KP_SUBTRACT        333
#define MAR_KEY_KP_ADD             334
#define MAR_KEY_KP_ENTER           335
#define MAR_KEY_KP_EQUAL           336
#define MAR_KEY_LEFT_SHIFT         340
#define MAR_KEY_LEFT_CONTROL       341
#define MAR_KEY_LEFT_ALT           342
#define MAR_KEY_LEFT_SUPER         343
#define MAR_KEY_RIGHT_SHIFT        344
#define MAR_KEY_RIGHT_CONTROL      345
#define MAR_KEY_RIGHT_ALT          346
#define MAR_KEY_RIGHT_SUPER        347
#define MAR_KEY_MENU               348

#define MAR_KEY_LAST               MAR_KEY_MENU

// --- MOUSE DEFINES --- //
#define MAR_MOUSE_BUTTON_1         0
#define MAR_MOUSE_BUTTON_2         1
#define MAR_MOUSE_BUTTON_3         2
#define MAR_MOUSE_BUTTON_4         3
#define MAR_MOUSE_BUTTON_5         4
#define MAR_MOUSE_BUTTON_6         5
#define MAR_MOUSE_BUTTON_7         6
#define MAR_MOUSE_BUTTON_8         7
#define MAR_MOUSE_BUTTON_LAST      MAR_MOUSE_BUTTON_8
#define MAR_MOUSE_BUTTON_LEFT      MAR_MOUSE_BUTTON_1
#define MAR_MOUSE_BUTTON_RIGHT     MAR_MOUSE_BUTTON_2
#define MAR_MOUSE_BUTTON_MIDDLE    MAR_MOUSE_BUTTON_3
