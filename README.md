# Vulkan 3D Graphics Engine

A modern C++ Vulkan-based 3D graphics engine that demonstrates fundamental computer graphics concepts, 3D rendering, and interactive camera controls. This project showcases a complete Vulkan application with a custom face model rendered in 3D space.

## 🎯 Project Overview

This Vulkan engine implements a complete graphics pipeline featuring:

- **3D Face Rendering**: Custom procedurally generated 3D face model with detailed facial features
- **Interactive Camera System**: First-person camera controls with keyboard movement
- **Modern Vulkan Architecture**: Clean, modular C++ design following Vulkan best practices
- **Real-time Rendering**: Smooth 60+ FPS rendering with proper frame synchronization

## 🏗️ Architecture

### Core Components

#### **FirstApp** (`first_app.hpp/cpp`)

The main application class that orchestrates the entire rendering pipeline:

- Manages the main render loop
- Handles window events and input
- Coordinates between all subsystems
- Creates and manages game objects

#### **LveWindow** (`lve_window.hpp/cpp`)

GLFW-based window management:

- Creates and manages the application window
- Handles window resize events
- Provides Vulkan surface creation
- Window properties: 800x600 pixels

#### **LveDevice** (`lve_device.hpp/cpp`)

Vulkan device and instance management:

- Vulkan instance creation with validation layers
- Physical and logical device selection
- Queue family management
- Memory allocation and buffer management
- Command pool creation

#### **LveRenderer** (`lve_renderer.hpp/cpp`)

High-level rendering coordinator:

- Manages swap chain and frame synchronization
- Command buffer allocation and submission
- Render pass management
- Frame timing and synchronization

#### **LveSwapChain** (`lve_swapchain.hpp/cpp`)

Swap chain and presentation management:

- Double-buffered rendering (2 frames in flight)
- Depth buffer management
- Image view creation
- Present mode selection

#### **LvePipeline** (`lve_pipeline.hpp/cpp`)

Graphics pipeline management:

- Shader module loading and compilation
- Pipeline state configuration
- Push constant handling
- Vertex input configuration

#### **LveModel** (`lve_model.hpp/cpp`)

3D model and vertex data management:

- Vertex buffer creation and management
- Vertex attribute descriptions
- Model rendering commands

#### **LveGameObject** (`lve_gameobject.hpp/cpp`)

Game object representation:

- Transform component (position, rotation, scale)
- Model association
- Unique ID system
- Move semantics for performance

#### **LveCamera** (`lve_camera.hpp/cpp`)

Camera system with multiple projection modes:

- Perspective and orthographic projections
- View matrix calculations
- Multiple view modes (target-based, direction-based, YXZ rotations)

#### **KeyboardMovementController** (`keyboard_movement_controller.hpp/cpp`)

Input handling system:

- WASD movement controls
- Arrow key camera look controls
- Configurable key mappings
- Frame-time based movement

#### **SimpleRenderSystem** (`simple_render_system.hpp/cpp`)

Basic rendering system:

- Object rendering loop
- Push constant updates
- Camera matrix application

## 🎨 3D Face Model

The project features a procedurally generated 3D face model with:

### Facial Features

- **Head**: Main rectangular block (skin tone: RGB 1.0, 0.85, 0.75)
- **Eyes**: White cubes with black pupils
- **Eyebrows**: Dark brown rectangular blocks
- **Nose**: Pyramid structure in center
- **Mouth**: Three-segment curved design (red color)

### Geometry Generation

- **Cube Generation**: Procedural cube creation with 6 faces
- **Pyramid Generation**: Triangular pyramid for nose structure
- **Color Coding**: Each face has distinct colors for visual clarity
- **Transform Operations**: Y and X axis flipping for proper orientation

## 🔧 Technical Specifications

### Dependencies

- **Vulkan SDK**: 1.4.313.0 (macOS)
- **GLFW**: Window management and input handling
- **GLM**: OpenGL Mathematics library for 3D math
- **Compiler**: Clang++ with C++17 standard

### Build System

- **Makefile**: Automated build with shader compilation
- **Shader Compilation**: Automatic GLSL to SPIR-V compilation using `glslc`
- **Optimization**: O2 optimization level for release builds

### Shaders

#### Vertex Shader (`simple_shader.vert`)

```glsl
#version 450
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
    mat4 transform;
    vec3 color;
} push;

void main() {
    gl_Position = push.transform * vec4(position, 1.0);
    fragColor = color;
}
```

#### Fragment Shader (`simple_shader.frag`)

```glsl
#version 450
layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    mat4 transform;
    vec3 color;
} push;

void main() {
    outColor = vec4(fragColor, 1.0);
}
```

## 🚀 Getting Started

### Prerequisites

- macOS (configured for macOS Vulkan SDK)
- Vulkan SDK 1.4.313.0 or compatible
- GLFW library
- GLM library
- Clang++ compiler

### Installation

1. **Install Dependencies**

   ```bash
   # Install GLFW and GLM via Homebrew
   brew install glfw glm
   ```

2. **Install Vulkan SDK**

   - Download from [LunarG Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
   - Extract to `/Users/[username]/VulkanSDK/1.4.313.0/macOS/`

3. **Update Makefile Paths**
   - Modify `VULKAN_SDK_PATH` in Makefile to match your installation
   - Update include/library paths if needed

### Building and Running

```bash
# Build the project
make

# Run the application
make test

# Clean build artifacts
make clean
```

### Manual Build

```bash
# Compile shaders
glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv
glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv

# Compile and link
clang++ -std=c++17 -O2 -Iinclude -I/opt/homebrew/include -I[VULKAN_SDK_PATH]/include \
  main.cpp src/*.cpp -L/opt/homebrew/lib -L[VULKAN_SDK_PATH]/lib \
  -lglfw -lvulkan -o build/VULKAN
```

## 🎮 Controls

### Camera Movement

- **W/A/S/D**: Move forward/left/backward/right
- **Q/E**: Move down/up
- **Arrow Keys**: Look around (pitch/yaw)

### Application

- **ESC**: Close application
- **Window Resize**: Automatically handled with swap chain recreation

## 📁 Project Structure

```
VULKAN/
├── include/                    # Header files
│   ├── first_app.hpp          # Main application class
│   ├── lve_window.hpp         # Window management
│   ├── lve_device.hpp         # Vulkan device management
│   ├── lve_renderer.hpp       # Rendering coordinator
│   ├── lve_swapchain.hpp      # Swap chain management
│   ├── lve_pipeline.hpp       # Graphics pipeline
│   ├── lve_model.hpp          # 3D model management
│   ├── lve_gameobject.hpp     # Game object system
│   ├── lve_camera.hpp         # Camera system
│   ├── keyboard_movement_controller.hpp # Input handling
│   └── simple_render_system.hpp # Basic render system
├── src/                       # Source files
│   └── [corresponding .cpp files]
├── shaders/                   # Shader files
│   ├── simple_shader.vert     # Vertex shader
│   ├── simple_shader.frag     # Fragment shader
│   └── *.spv                  # Compiled SPIR-V shaders
├── build/                     # Build artifacts
│   ├── VULKAN                 # Executable
│   └── compile_commands.json  # Compilation database
├── output/                    # Additional output
├── main.cpp                   # Application entry point
├── Makefile                   # Build configuration
└── README.md                  # This file
```

## 🔍 Key Features

### Vulkan Best Practices

- **Resource Management**: Proper cleanup and RAII patterns
- **Memory Management**: Efficient buffer and image allocation
- **Synchronization**: Proper semaphore and fence usage
- **Validation Layers**: Debug validation in development builds

### Performance Optimizations

- **Double Buffering**: 2 frames in flight for smooth rendering
- **Move Semantics**: Efficient object transfers
- **Static Allocation**: Pre-allocated command buffers
- **Push Constants**: Efficient uniform data transfer

### Extensibility

- **Modular Design**: Clean separation of concerns
- **Component System**: Flexible game object architecture
- **Shader System**: Easy shader replacement and modification
- **Input System**: Configurable key mappings

## 🐛 Troubleshooting

### Common Issues

1. **Vulkan SDK Not Found**

   - Verify Vulkan SDK installation path
   - Update `VULKAN_SDK_PATH` in Makefile

2. **GLFW/GLM Not Found**

   - Install via Homebrew: `brew install glfw glm`
   - Check include/library paths

3. **Shader Compilation Errors**

   - Ensure `glslc` is in PATH
   - Check shader syntax (GLSL 4.50)

4. **Runtime Errors**
   - Verify Vulkan drivers are installed
   - Check validation layer messages
   - Ensure proper GPU support

### Debug Mode

The project includes validation layers for debugging:

- Enable with `NDEBUG` undefined
- Provides detailed Vulkan error messages
- Helps identify resource leaks and invalid operations

## 🎓 Learning Objectives

This project demonstrates:

- **Vulkan API Fundamentals**: Instance, device, and queue management
- **Graphics Pipeline**: Complete pipeline setup and configuration
- **Memory Management**: Buffer creation and vertex data handling
- **3D Mathematics**: Transform matrices and camera systems
- **Input Handling**: Real-time user interaction
- **Modern C++**: RAII, move semantics, and smart pointers

## 🔮 Future Enhancements

Potential improvements and extensions:

- **Texture Support**: Add texture mapping capabilities
- **Lighting System**: Implement Phong or PBR lighting
- **Model Loading**: Support for external 3D model formats
- **Animation System**: Keyframe and skeletal animation
- **Physics Integration**: Add physics simulation
- **Multi-Platform Support**: Windows and Linux compatibility

## 📚 References

- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Specification](https://www.khronos.org/registry/vulkan/specs/1.3/html/)
- [GLFW Documentation](https://www.glfw.org/docs/)
- [GLM Documentation](https://glm.g-truc.net/0.9.9/index.html)

## 📄 License

This project is for educational purposes. Please ensure compliance with Vulkan SDK and library licenses when using in commercial projects.

---

**Note**: This project is configured for macOS. For other platforms, update the Makefile paths and ensure proper Vulkan SDK installation for your target platform.
