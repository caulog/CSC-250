# simple makefile for clang-tidy and compiling
#
# Nirre Pluf 
# 9/5/2021
# CSC 250 Lab 2

SOURCE = lab2.c
EXECUTABLE = lab2
COMPILE_FLAGS = -ansi -pedantic -Wall

all: tidy
	gcc $(COMPILE_FLAGS) -o $(EXECUTABLE) $(SOURCE)

tidy: $(SOURCE)
	clang-tidy -checks='*' $(SOURCE) -- -std=c99

clean:
	rm -rf $(EXECUTABLE)

