#All compiles everything

CXXFLAGS = -lGL -lGLU -lglut -lm -o
CFLAGS = -Wall -pedantic -ansi

all: main

main: StepModel.cpp
		g++ StepModel.cpp RgbImage.cpp $(CXXFLAGS) main
		./main
clean: 
		rm -rf *o main