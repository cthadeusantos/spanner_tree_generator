# Define the symbols we might want to change:
SRCS_DIR := src/
CODE_DIR := $(SRCS_DIR)code/
BUILD_DIR := build/
CXX := g++
CXXFLAGS := -std=c++11 -pthread -g -O3 -w
input := main.cpp
output := exec.out

all:
	clear
	$(CXX)  $(CXXFLAGS) ${CODE_DIR}*.cpp $(SRCS_DIR)$(input) -o $(BUILD_DIR)$(output)
