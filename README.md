# Toy Real Time PBR Engine
- Desktop App
- Language: C++17
- Graphics API: OpenGL

- Implementing Effects
  - 1 Rasterization Rendering (IBL, Deferred Lighting, Ambient Occlusion, ETC)
    - Render Dependency Graph
      ![image](https://github.com/Windowline/Toy-PBR-Engine/assets/17508384/ba922aba-9588-434f-92d3-136f4fefdfbf)

    - [Metalic: 0.9 / Roughness: 0.1]
      ![image](https://github.com/Windowline/ToyRenderer/assets/17508384/28df3a4e-9c4d-4cca-bb7d-85bf422d8151)

    - [Metalic: 0.6 / Roughness: 0.4]
      ![image](https://github.com/Windowline/ToyRenderer/assets/17508384/9166d4bf-ed49-4649-96b0-6dce5239d380)
      
  - 2 Ray Tracing Rendering (BVH, ETC) (Doing)
      ![image](https://github.com/user-attachments/assets/269ab8cb-f933-4c4b-9ace-7984c17ab1b3)


- Camera Control UX
  - Mouse: Rotate view-space basis 
  - Keybord
    - W: Move to Z of view-space
    - S: Move to -Z of view-space
    - A: Move to -X of view-space
    - D: Move to X of view-space


## Build Tools
- CMake 3.2
- vcpkg

## Install Dependencies
```
./vcpkg install glad
./vcpkg install glfw
./vcpkg install ASSIMP
./vcpkg install stb
```

## Build
```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="{your_vcpkg_path}/scripts/buildsystems/vcpkg.cmake"
make
```

## Run Sample App
Rasterization Rendering
```
./app_pbr
```

Ray Tracing Rendering
```
./app_ray_trace
```


  
