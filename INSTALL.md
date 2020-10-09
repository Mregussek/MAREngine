# How to install MAREngine

The version, currently the only one available, is prepared for Windows 10 32-bit. OpenGL libraries (32-bit versions) are downloaded and added by default to linker, so you do not have to download them again. Only dependency is actually this repository.

## Dependencies

The most needed application for MAREngine on Windows is Visual Studio 2019. Through it I am able to compile sources correctly. CMake is also added for future integration with Linux Operating Systems.

Some requirements are fullfiled from public repositories, but some of them are just added.

Repositories, that need to be cloned (if clone MAREngine with --recurse-submodules you will have them automatically):

- [ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit/tree/master)
- [ImGuiFileDialog](https://github.com/BalazsJako/ImGuiColorTextEdit/tree/master)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo/tree/master)
- [MARMaths](https://github.com/Mregussek/MARMaths/tree/master)
- [OBJ-Loader](https://github.com/Mregussek/OBJ-Loader/tree/master)
- [dirent](https://github.com/tronkko/dirent/tree/master)
- [imgui (docking branch)](https://github.com/ocornut/imgui/tree/docking)
- [pybind11](https://github.com/pybind/pybind11/tree/master)
- [spdlog](https://github.com/gabime/spdlog/tree/master)

Repositories, that are added already with the MAREngine:

- glew
- glfw3
- entt (because it is single-include, we don't need the whole repository)
- python3.8.5
- stb_image

## Installation

Please, do not download repository as a .zip file, because you will not fullfil dependencies such as submodules above!

## Windows

### #1 The easiest way

1. Clone repository

```bash
git clone --recurse-submodules https://github.com/mregussek/MAREngine.git
```

2. Open MAREngine.sln (located in main directory).

<p align="center">
  <img width="621" height="91" src="img/marenginesln.jpg">
</p>

3. Make sure, that selected target is x86 (no x64!)

<p align="center">
  <img width="374" height="34" src="img/x86.jpg">
</p>

4. Firstly you will have to build *MAREngine* in order to create static library.

<p align="center">
  <img width="265" height="96" src="img/compile-static-marengine.jpg">
</p>

<p align="center">
  <img width="369" height="59" src="img/build-static-marengine.jpg">
</p>

5. If library is compiled succesfully you should find ${SolutionDir}/bin/Release/Engine/MAREngine.lib file. Now you can compile either **EditorMAR** or **SandboxMAR**. Choose, which one you want to use with *Build* option. Also make sure, that selected project should be set as startup (marked with bold font).

<p align="center">
  <img width="350" height="69" src="img/build-exe-editormar.jpg">
</p>

6. Have fun! ;)

### #2 CMake on Windows

1. Clone repository (with submodules, cause I use ImGui, spdlog, serialport and MARMaths from its own sources!)

```
git clone --recurse-submodules https://github.com/mregussek/MAREngine.git
```

2. Open CMake (CMake-GUI in my case)
3. Select proper paths!

- Where is the source code: Here please select path, where you cloned repository!
- Where to build binaries: Please select path to repository with /build directory!

Examples:

- Where is the source code: *C:/Path/to/MAREngine*
- Where to build binaries: *C:/Path/to/MAREngine/build*

4. Press **Generate** button!

<p align="center">
  <img width="813" height="356" src="img/cmake_generate.jpg">
</p>

5. New window will be opened. Here make sure, that you choose generator for Win32!)

<p align="center">
  <img width="504" height="378" src="img/win32cmake.jpg">
</p>

6. Press **Finish**. CMake will generate all needed files.
7. Select **Open Project** button. You will be able to compile project.
8. Firstly you have to build *MAREngine* in order to create static library.

<p align="center">
  <img width="284" height="460" src="img/cmake-project-list.jpg">
</p>

9. Secondly you have to build **EditorMAR** or **SandboxMAR**. Make sure, that selected project should be set as startup (marked with bold font).

10. Copy *DefaultProject*, *resources* directories from EditorMAR to *C:/Path/to/MAREngine/build*. Also copy desktop.ini, imgui.ini and python38 to *C:/Path/to/MAREngine/build*.
