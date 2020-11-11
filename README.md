
<img src="./res/title.png" width="500" >

# 

![Version](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/Haeri/PhotonBox/master/vcpkg.json&label=version&query=$['version-string']&color=blue)
[![Actions Status](https://github.com/Haeri/PhotonBox/workflows/C++%20Builder/badge.svg)](https://github.com/Haeri/PhotonBox/actions)
[![License](https://img.shields.io/github/license/Haeri/PhotonBox.svg)](https://github.com/Haeri/PhotonBox/blob/master/LICENSE)

A portable modern OpenGL Game-Engine with focus on PBR

## Project Goal

The PhotonBox Engine aims to be an open source resource for people interested in graphics and system architecture. The goal is to create a clean and readable codebase, whilst still maintaining good performance and a high graphical fidelity. An Editor is currently not planned as I am still exploring the possibilities for state based GUI solutions.

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
- PhysX 3.4 partial integration
- ImGui integration

## Project Setup

### Requirements
- [CMake 3.3^](https://cmake.org/download/) 
- [Python 3^](https://www.python.org/downloads/)

### <img height="14" src="https://image.flaticon.com/icons/svg/888/888882.svg"> Windows

Generate the visual studio project file:
```cmd
git clone https://github.com/Haeri/PhotonBox.git --recursive
cd PhotonBox
./tools/setup_win.bat
```
<sub>Make sure to open the Visual Studio project as administrator, as it needs to perform copy operatiosn to make resources files available to the binaries.</sub>


### <img height="16" src="https://image.flaticon.com/icons/svg/226/226772.svg"> Linux

Generate the make files:
```sh
git clone https://github.com/Haeri/PhotonBox.git --recursive
cd PhotonBox
sudo apt update
sudo apt install -y mesa-common-dev mesa-utils libgl1-mesa-dev
sudo apt install -y cmake curl unzip tar 
./tools/setup_nix.sh
```


### <img height="16" src="https://image.flaticon.com/icons/svg/2/2235.svg"> Mac

Generate the xcode project file:
```bash
git clone https://github.com/Haeri/PhotonBox.git --recursive
cd PhotonBox
./tools/setup_nix.sh
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
