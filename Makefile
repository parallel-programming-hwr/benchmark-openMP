all: main

main:
	gcc -fopenmp -std=c11 -pedantic -Wall -lm -o run main.c
