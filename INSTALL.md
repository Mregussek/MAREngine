# How to install MAREngine

The version, currently the only one available, is prepared for Windows 10 64-bit. OpenGL libraries (64-bit versions) are downloaded and added by default to linker, so you do not have to download them again. Only dependency is actually this repository.

## Software

- Visual Studio 2019

I have prepared solution file for Visual Studio 2019, so it will be wise to run this on VS2019. I am using free, community version. It containts everything needed for this project.

I will soon write CMake files for Linux version of MAREngine.

## Dependencies

All requirements are added to this repository!

- OpenGL (4.6.0 used in this project)
- stb-image (v2.25)
- Dear ImGui (v1.76)

## Installation

1. Create new project and choose to clone the repository.
2. In Repository Localization paste: *[https://github.com/Mregussek/MAREngine.git](https://github.com/Mregussek/MAREngine)*
3. VS2019 should do everything for us (download project and prepare a directory).
4. After cloning repository you must open file named **mar.h**. Path is *"/MAREngine/src/mar.h"*. There you fill a line, which asks you, if you imported repository from github. Please, uncomment this line and then you will be able the project properly.
