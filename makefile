# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Iinclude -I/usr/local/include -std=c++17 -DGL_SILENCE_DEPRECATION

# Linker flags
LDFLAGS = -lglfw -ldl -framework OpenGL

# Source files
SRCS = src/main.cpp src/glad.c

# ensure skybox shaders are copied next to binary (optional rule could be added)

# Output executable
TARGET = bin/SnakeMacOS

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET) *.o