# How to install MAREngine

The version, currently the only one available, is prepared for Windows 10 32-bit. OpenGL libraries (32-bit versions) are downloaded and added by default to linker, so you do not have to download them again. Only dependency is actually this repository.

# Software

The most needed application for MAREngine on Windows is Visual Studio 2019. Through it I am able to compile sources correctly. CMake is also added 
for future integration with Linux Operating Systems.

# Dependencies

All requirements are added to this repository! You need to compile sources for your machine!

- OpenGL (4.6.0 used in this project)
- stb-image (v2.25)
- Dear ImGui (v1.77 from docking branch)
- spdlog (1.6.0)
- SerialPort

# Installation

Please, do not download repository as a .zip file, because you will not fullfil dependencies such as submodules above!

## Windows

### #1 The easiest way

1. Clone repository (with submodules, cause I use ImGui, spdlog, serialport and MARMaths from its own sources!)

```
git clone --recurse-submodules https://github.com/mregussek/MAREngine.git
```

2. Open MAREngine.sln (located in main directory).

<p align="center">
  <img width="500" height="100" src="img/marenginesln.jpg">
</p>

3. Make sure, that selected target is x86 (no x64!!!)

<p align="center">
  <img width="382" height="50" src="img/x86.jpg">
</p>

4. Firstly you will have to compile *MAREngine* in order to create static library.

<p align="center">
  <img width="300" height="175" src="img/compile-library.jpg">
</p>
<p align="center">
  <img width="350" height="50" src="img/compile-library-explicit.jpg">
</p>

5. If library is compiled succesfully you should find ${SolutionDir}/bin/Release/Engine/MAREngine.lib file. 
Now you can compile either EditorMAR or SandboxMAR. Choose, which one you want to use with *Compile Again* option.

<p align="center">
  <img width="350" height="100" src="img/compile-again-editor.jpg">
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
  <img width="800" height="400" src="img/cmake_generate.jpg">
</p>

5. New window will be opened. Here make sure, that you choose generator for Win32!)

<p align="center">
  <img width="500" height="400" src="img/win32cmake.jpg">
</p>

6. Press **Finish**. CMake will generate all needed files.
7. Select **Open Project** button. You will be able to compile project.
8. Select **EditorMAR** or **SandboxMAR** as a starting project. Then project should be marked as bold.
EditorMAR and SandboxMAR are compiled for being executable files, MAREngine on the other hand is a library, so its
compilation will create static library.

<p align="center">
  <img width="300" height="175" src="img/starting.jpg">
</p>

9. Compile ;)

