# Custom OpenGL 3D Renderer

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg)
![License](https://img.shields.io/badge/license-BSL-orange.svg)

<!-- 
TODO: Add your demo GIF/video here
![Demo](assets/demo.gif)
-->

https://github.com/user-attachments/assets/02a66141-8326-4ae2-9fb8-1639439a29ee

https://github.com/user-attachments/assets/65ba31a1-94fc-4a24-b00f-4876b3e0be94

<img width="1918" height="990" alt="Screenshot 2026-02-21 204409" src="https://github.com/user-attachments/assets/8f6adf72-11cf-407f-97a7-0ce6043b3860" />

<img width="1880" height="1000" alt="Screenshot 2026-02-21 204525" src="https://github.com/user-attachments/assets/7e5205e5-1e98-4d0d-8c32-f1bb604e2704" />

<img width="1900" height="1004" alt="Screenshot 2026-02-21 204629" src="https://github.com/user-attachments/assets/326fbc88-923c-4233-b98d-b1da13a9e089" />

<img width="1920" height="993" alt="Screenshot 2026-02-21 204838" src="https://github.com/user-attachments/assets/48ee94be-5aba-426a-8ebe-2e1ee9c1ebed" />

<img width="1920" height="989" alt="Screenshot 2026-02-21 205027" src="https://github.com/user-attachments/assets/3df9d8f7-f0a8-4ae9-ae0e-a72cf1cc80f0" />



## Overview

A custom 3D rendering engine built from scratch in C++ and OpenGL. Designed to explore low-level GPU programming, advanced lighting techniques, and real-time shadow rendering. This project demonstrates proficiency in graphics pipeline management, memory optimization, and shader-based rendering architectures.

---

## Core Graphical Features

### Rendering Techniques
- **Blinn-Phong Lighting Model**: Physically-based shading with ambient, diffuse, and specular components
- **Multi-Pass Shadow Rendering**:
  - Directional shadow mapping with depth texture optimization
  - Omnidirectional point light shadows using depth cubemaps
  - Percentage-Closer Filtering (PCF) for soft shadow edges
- **Skybox Rendering**: Environment mapping using cubemap textures
- **Blinn-Phong Material System**: Support for diffuse and specular texture channels

### Asset Pipeline
- **Model Loading**: Hierarchical mesh loading via Assimp (`.obj`, `.gltf`, `.fbx`)
- **Automatic Material Binding**: Material properties and textures automatically mapped from imported assets
- **Texture Management**: Efficient `stb_image` integration with OpenGL texture slots

### Camera System
- **Free-Look Camera**: WASD movement with mouse-driven pitch/yaw control
- **Smooth Interpolation**: Delta-time based movement for frame-rate independent motion

---

## Technical Architecture

### GPU Memory Management
Implemented a custom abstraction layer for OpenGL buffer objects to ensure efficient vertex data transfer:
- **Vertex Buffer Objects (VBO)**: Contiguous memory layout for vertex attributes (position, normal, UV)
- **Element Buffer Objects (EBO)**: Indexed drawing to reduce redundant vertex data
- **Vertex Array Objects (VAO)**: State container pattern to minimize driver overhead during draw calls

### Shadow System Design
**Problem**: Real-time omnidirectional shadows from point lights require rendering the scene 6 times per light (one for each cubemap face).

**Solution**: 
- Implemented a geometry shader-based approach that renders to all 6 faces in a single pass
- Used framebuffer objects (FBO) with depth-only attachments to optimize bandwidth
- Applied shadow acne mitigation through depth bias tuning and PCF sampling

### OpenGL State Management
**Problem**: Texture bleeding artifacts caused by unbound texture slots persisting between draw calls.

**Solution**: 
- Strict texture slot lifecycle management: explicitly unbind `GL_TEXTURE0-N` after each material pass
- Centralized shader uniform binding through a `ShaderProgram` wrapper class
- Debug assertions to catch state leakage during development

---

## Tech Stack

| Category | Technology |
|:---------|:-----------|
| **Language** | C++17 |
| **Graphics API** | OpenGL 3.3 Core |
| **Mathematics** | GLM (OpenGL Mathematics) |
| **Windowing** | GLFW 3 |
| **Model Loading** | Assimp |
| **Texture Loading** | stb_image |
| **UI** | Dear ImGui |
| **Build System** | Visual Studio / MSBuild |

---

## Project Structure

```
GL_3D_Renderer/
├── src/
│   ├── buffer/          # VBO, VAO, EBO, FBO abstractions
│   ├── GUI/             # Graphical user interface with DearImGui
│   ├── lighting/        # Light classes (Point, Directional) and shadow systems
│   ├── model/           # Model and Mesh classes with Assimp integration
│   ├── resource/        # Texture and Cubemap loaders
│   ├── shaders/         # GLSL vertex/fragment shaders
│   ├── vertex/          # Vertex classes and struct definitions
│   ├── camera.cpp       # Camera system implementation
│   ├── scene.cpp        # Main render loop and scene graph
│   └── main.cpp         # Entry point
├── assets/              # Model assets
└── dependencies/        # Third-party libraries
```

---

## Build Instructions

### Prerequisites
- **Visual Studio 2022** (or 2019 with C++17 support)
- **Windows 10/11** (tested environment)
- **Git**

### Clone and Build

```bash
# Clone the repository
git clone https://github.com/nhatkhoa1124/GL_3D_Renderer.git
cd GL_3D_Renderer

# Open the solution in Visual Studio
start GL_3D_Renderer.sln

# Build the project (Ctrl+Shift+B)
# Or use MSBuild from command line:
msbuild GL_3D_Renderer.sln /p:Configuration=Release
```

### Run the Engine

```bash
# Run from Visual Studio (F5)
# Or execute the binary directly:
.\GL_3D_Renderer\GL_3D_Renderer.exe
```

---

## Controls

| Input | Action |
|:------|:-------|
| **W/A/S/D** | Camera movement |
| **Mouse** | Camera look direction |
| **ESC** | Close application |

---

## Key Learning Outcomes

### Graphics Programming
- Deep understanding of the OpenGL rendering pipeline (vertex processing → rasterization → fragment shading)
- Practical experience with multi-pass rendering architectures
- Proficiency in GLSL shader development (vertex, fragment, geometry stages)

### C++ Systems Design
- RAII-based resource management for GPU objects (smart pointers for lifetime control)
- Object-oriented design patterns for scene graph hierarchies
- Performance-conscious memory layouts (structure-of-arrays vs array-of-structures trade-offs)

### Problem Solving
- Diagnosed and fixed shadow acne, Peter panning, and texture bleeding artifacts
- Optimized draw call batching to reduce CPU-GPU synchronization overhead
- Debugged OpenGL state machine issues using RenderDoc and driver error callbacks


---

## References & Resources

- [LearnOpenGL](https://learnopengl.com/) - Foundational graphics programming tutorials
- [OpenGL Specification](https://www.khronos.org/opengl/) - Official API documentation
- [Real-Time Rendering, 4th Edition](https://www.realtimerendering.com/) - Advanced rendering techniques

---

## License

This project is licensed under the BSL License - see the [LICENSE](LICENSE) file for details.
