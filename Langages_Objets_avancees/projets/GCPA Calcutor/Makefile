CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

SRCDIR := src
BINDIR := bin
TARGET := cgpa

# Default target first
all: $(TARGET)

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BINDIR) $(TARGET)

.PHONY: all clean
