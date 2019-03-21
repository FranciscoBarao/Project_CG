#All compiles everything

CXXFLAGS = -lGL -lGLU -lglut -lm -o
CFLAGS = -Wall -pedantic -ansi

all: main

main: StepModel.cpp
		g++ StepModel.cpp rgb $(CXXFLAGS) main
		./main

rgb:
	RgbImage.cpp
clean: 
		rm -rf *o main