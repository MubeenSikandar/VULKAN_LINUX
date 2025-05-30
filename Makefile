CXX = g++
CFLAGS = -std=c++17 -O2

# Link with common Vulkan and GLFW dependencies for X11
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Auto-detect sources
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = VULKAN

# Default build target
all: $(TARGET)

# Link object files to create the final binary
$(TARGET): $(SOURCES)
	$(CXX) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
