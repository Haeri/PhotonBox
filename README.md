# PhotonBox
A modern OpenGL Game-Engine with focus on PBR

## Rendering 
- Forward Rendering
- Mixed Deferred Rendering
- Point lights, Directional lights, Spot lights
- SkyBox/Enviroment Mapping
- Cook Torrance BRDF shader
  - Albedo Map
  - Normal Map
  - Roughness Map
  - Metallic Map
  - Ambient Occclusion Map
  - Emission Map
- Dynamic Irradiance Map generation
- Dynamic Specular-Convolution Map generation
- Post Processing List
  - Eye adaptation
  - Bloom
  - Tone mapping
  - SSAO
  - SSReflections
- Transparent rendering
- Cutout rendering
- Shadow mapping
- Text rendering
- Parallax cubeMap correction  

## Systems
- Component system
- Scene loading
- Transformation system
- Generic Materials
- Input
- Lightprobe volumes
- Mesh caching
- PhysX 3.4 integration
- ImGui integration
- Shader hot reload

## Media
Interiour
![screenshot_1](https://user-images.githubusercontent.com/7956606/35122763-2d3a2934-fca0-11e7-8f38-552fcc106b7e.png)

Automotive
![screenshot 2](https://user-images.githubusercontent.com/7956606/43651128-d3b01440-9741-11e8-9224-c5bded3dedce.png)
## Dependency

NVIDIA PhysX 3.4
Header files required from https://github.com/NVIDIAGameWorks/PhysX-3.4
(To get access to the repository, you will need to accept the [PhysX EULA](https://developer.nvidia.com/content/apply-access-nvidia-physx-source-code))
