NAME = TSAnalyzer
CFLAGS = -Wall -Wextra -g
CXX = g++

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
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)
	
clean:
	rm -f $(SRCDIR)/*.o
	rm -f $(TARGET)

.PHONY: all run clean