CXX = clang++
CFLAGS = -std=c++17 -O2

# Manually specify Vulkan SDK path
VULKAN_SDK_PATH = /Users/mubeensikandar/VulkanSDK/1.4.313.0/macOS
INCLUDES = -I/opt/homebrew/include -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L/opt/homebrew/lib -L$(VULKAN_SDK_PATH)/lib -lglfw -lvulkan

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = VULKAN

$(TARGET): $(SOURCES)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) $(LDFLAGS)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
