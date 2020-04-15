all: main

main:
	gcc -fopenmp -std=c11 -pedantic -Wall  -o run main.c -lm
