all:
	gcc main.c -std=c99 -pedantic -g -Wall -o phong -lm -lSDL2
clear:
	rm ./phong
