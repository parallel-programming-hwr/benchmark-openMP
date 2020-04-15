all: main

main:
	gcc -std=c11 -Wextra -pedantic -Wall -lm -o run main.c
