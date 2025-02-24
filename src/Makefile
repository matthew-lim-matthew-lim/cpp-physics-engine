# Useful Resource: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -O3 -Wall -Wextra -std=c++20

# Libraries
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

# Source files
SRC = main.cpp Ui/Slider.cpp Utility/Vec.cpp Utility/LTexture.cpp Shapes/Shape.cpp Shapes/Rectangle.cpp Shapes/Circle.cpp 

# Put all the obj files in an `obj` directory so the codebase isn't filled with them
ODIR = obj

# Object files (convert .cpp -> .o and place in ODIR)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

# Executable name
TARGET = program

# Default target
all: $(TARGET)

# Link all object files into the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Ensure the object directory exists before compiling
$(ODIR)/%.o: %.cpp
	@mkdir -p $(dir $@) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(ODIR) $(TARGET)