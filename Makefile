all:
	gcc main.c -O2 -std=c99 -pedantic -g -Wall -o phong -lm -lSDL2 -lSDL2_gfx
clear:
	rm ./phong
