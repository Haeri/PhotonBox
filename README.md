
<img src="./res/title.png" width="500">

![Version](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/Haeri/PhotonBox/master/vcpkg.json&label=version&query=$['version-string']&color=blue)
[![Actions Status](https://github.com/Haeri/PhotonBox/workflows/C++%20Builder/badge.svg)](https://github.com/Haeri/PhotonBox/actions)
[![License](https://img.shields.io/github/license/Haeri/PhotonBox.svg)](https://github.com/Haeri/PhotonBox/blob/master/LICENSE)

A portable modern OpenGL Game-Engine with focus on PBR

## Project Goal

The PhotonBox Engine aims to be an open source resource for people interested in computer graphics. The goal is to create a clean and readable codebase, whilst still maintaining good performance and a high graphical fidelity. An Editor is currently not planned as I am still exploring the possibilities for state based GUI solutions.

## Rendering

- Deferred/Forward Rendering
- Point lights, Directional lights, Spot lights
- SkyBox/Enviroment Mapping
- Parallax cubeMap correction
- Cook Torrance BRDF shader
  - Albedo Map
  - Normal Map
  - Roughness Map
  - Metallic Map
  - Ambient Occclusion Map
  - Emission Map
- Dynamic Irradiance & Specular-Convolution generation
- Post Processing List
  - Eye adaptation
  - Bloom
  - Tone mapping
  - SSAO
  - SSReflections
  - Temporal anti aliasing
- Transparent/Cutout rendering
- Shadow mapping
- Contact shadows
- Volumetric Fog
- Text rendering

## Systems

- Component system
- Scene system
- Generic Materials
- Lightprobe volumes
- Resource caching
- Async resource loading
- Shader hot reload
- PhysX partial integration
- ImGui integration

## Project Setup

### Requirements
- [CMake 3.16^](https://cmake.org/download/) 
- [Python 3^](https://www.python.org/downloads/)
- OpenGL 4.1

### <img height="14" src="https://cdn-icons-png.flaticon.com/512/220/220215.png"> Windows

Generate a visual studio project file:
```cmd
git clone https://github.com/Haeri/PhotonBox.git --recursive
cd PhotonBox
./scripts/setup_win.bat
```
<sub>Make sure to open the Visual Studio project as administrator, as it needs to perform copy operatiosn to make resources files available to the binaries.</sub>


### <img height="18" src="https://cdn-icons-png.flaticon.com/512/226/226772.png"> Linux

Generate a make file:
```sh
git clone https://github.com/Haeri/PhotonBox.git --recursive
cd PhotonBox
sudo apt update
sudo apt install -y mesa-common-dev mesa-utils libgl1-mesa-dev cmake curl unzip tar 
./scripts/setup_nix.sh
```


### <img height="18" src="https://cdn-icons-png.flaticon.com/512/179/179309.png"> macOS

Generate the make project files:
```bas
git clone https://github.com/Haeri/PhotonBox.git --recursive
cd PhotonBox
./scripts/setup_nix.sh
```

## Media

Interiour
![screenshot_1](https://user-images.githubusercontent.com/7956606/35122763-2d3a2934-fca0-11e7-8f38-552fcc106b7e.png)
<sub>(Assets not distributed)</sub>

Automotive
![screenshot 2](https://user-images.githubusercontent.com/7956606/43651128-d3b01440-9741-11e8-9224-c5bded3dedce.png)


## Roadmap

### RENDERING

- Instanced rendering
- Deferred decals
- GTAO
- Anisotropic filtering

### GENERAL

- Precompile shader
- Abstraction layer over OpenGL
- Precompiled headers
- Switch to dynamic library compilation
- Decoupling asset conversion from engine
- Doxygen integration for documentation

### OPTIMIZATION

- Uniform buffer
- SIMD math
