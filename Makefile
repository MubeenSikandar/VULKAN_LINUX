CXX = clang++
CFLAGS = -std=c++17 -O2

# Manually specify Vulkan SDK path
VULKAN_SDK_PATH = /Users/mubeensikandar/VulkanSDK/1.4.313.0/macOS
INCLUDES = -I/opt/homebrew/include -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L/opt/homebrew/lib -L$(VULKAN_SDK_PATH)/lib -lglfw -lvulkan
GLSLC = $(VULKAN_SDK_PATH)/bin/glslc

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = VULKAN
$(TARGET): $(vertObjFiles) $(fragObjFiles)

$(TARGET): $(SOURCES)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	rm -f *.spv
