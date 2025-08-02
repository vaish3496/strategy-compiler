CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SOURCES = src/*.cpp
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean 