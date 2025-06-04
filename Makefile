# Compiler and Flags
CXX = clang++
CFLAGS = -std=c++17 -O2

VULKAN_SDK_PATH = /Users/mubeensikandar/VulkanSDK/1.4.313.0/macOS
INCLUDES = -Iinclude -I/opt/homebrew/include -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L/opt/homebrew/lib -L$(VULKAN_SDK_PATH)/lib -lglfw -lvulkan -Wl,-rpath,$(VULKAN_SDK_PATH)/lib
GLSLC = $(VULKAN_SDK_PATH)/bin/glslc

# Paths
SHADER_DIR = shaders
BUILD_DIR = build
TARGET = $(BUILD_DIR)/VULKAN

# Shader source and SPIR-V targets
VERT_SHADERS := $(wildcard $(SHADER_DIR)/*.vert)
FRAG_SHADERS := $(wildcard $(SHADER_DIR)/*.frag)
SPV_SHADERS := $(VERT_SHADERS:.vert=.vert.spv) $(FRAG_SHADERS:.frag=.frag.spv)

# Source files
SOURCES := main.cpp $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(SOURCES)))

# Default target
all: $(TARGET)

# Build binary
$(TARGET): $(SOURCES) $(SPV_SHADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o $@
	@echo "Build complete."

# Shader compilation rule
%.spv: %
	@echo "Compiling shader: $< -> $@"
	$(GLSLC) $< -o $@

# Run test
test: $(TARGET)
	DYLD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib ./$(TARGET)

# Clean rule (preserves shaders/*.vert and *.frag)
clean:
	rm -f $(BUILD_DIR)/VULKAN $(BUILD_DIR)/*.o $(SHADER_DIR)/*.spv

.PHONY: all clean test
