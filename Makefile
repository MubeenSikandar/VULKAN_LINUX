CXX = clang++
CFLAGS = -std=c++17 -O2

VULKAN_SDK_PATH = /Users/mubeensikandar/VulkanSDK/1.4.313.0/macOS
INCLUDES = -Iinclude -I/opt/homebrew/include -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L/opt/homebrew/lib -L$(VULKAN_SDK_PATH)/lib -lglfw -lvulkan
GLSLC = $(VULKAN_SDK_PATH)/bin/glslc

# Shader compilation
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

# Source files
SOURCES = main.cpp $(wildcard src/*.cpp)
OBJECTS = $(patsubst %.cpp, build/%.o, $(notdir $(SOURCES)))

TARGET = build/VULKAN

# Compile binary
$(TARGET): $(SOURCES) $(vertObjFiles) $(fragObjFiles)
	@mkdir -p build
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Shader rule
%.spv: %
	$(GLSLC) $< -o $@

.PHONY: clean test

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f build/VULKAN *.spv
