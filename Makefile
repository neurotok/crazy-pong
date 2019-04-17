all:
	gcc main.c SDL_FontCache.c -std=c99 -g -Wall -o phong -lSDL2 -lSDL2_ttf
clear:
	rm ./phong
