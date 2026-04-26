CXX = g++
CXXFLAGS = -std=c++23

TARGET = tree-viewer

all: 
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET) 
