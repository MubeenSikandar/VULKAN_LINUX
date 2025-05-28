CXX = g++
CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

VULKAN: $(SOURCES)
	$(CXX) $(CFLAGS) -o VULKAN $(SOURCES) $(LDFLAGS)

.PHONY: test clean

test: VULKAN
	./VULKAN

clean:
	rm -f VULKAN *.o
