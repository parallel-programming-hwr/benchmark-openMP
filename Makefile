all: main

main:
	gcc -std=c11 -Wall -lm -o run main.c
