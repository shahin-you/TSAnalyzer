NAME = TSAnalyzer
CXX = g++
CXXFLAGS = -std=c++17 -DNDEBUG -O2 -Wall -Wextra -g

# Directories
SRCDIR  := .
BINDIR  := ../bin
TARGET  := $(BINDIR)/$(NAME)

# Find all source files in the current directory
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run: $(TARGET)
	./$(TARGET)
	
clean:
	rm -f $(SRCDIR)/*.o
	rm -f $(TARGET)

.PHONY: all run clean