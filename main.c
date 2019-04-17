#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define WINDOW_WIDTH (350)
#define WINDOW_HEIGHT (500)

void gameLoop(void *arg);

int main(void)
{
	SDL_DisplayMode current;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GetCurrentDisplayMode(0, &current);
	
	//unsigned int wp = 100 + WINDOW_WIDTH + 50;

	unsigned int x = (current.w - (WINDOW_WIDTH * 2 + 100)) / 2;
	unsigned int y = (current.h - WINDOW_HEIGHT) / 2;
	SDL_Window *left_window = SDL_CreateWindow("Pong", x, y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_Window *right_window = SDL_CreateWindow("Pong", x + WINDOW_WIDTH + 100, y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);


	SDL_Rect player_1  = {20,WINDOW_HEIGHT / 2 - 25,10,50};
	SDL_Rect player_2 = {WINDOW_WIDTH - 30,WINDOW_HEIGHT / 2 - 25, 10, 50};

	SDL_Rect left_ball= {WINDOW_WIDTH / 2 - 5,WINDOW_HEIGHT / 2,  10, 10};
	SDL_Rect right_ball= {-10, 0,  10, 10};

	bool is_left = true;
	
	SDL_Renderer *left = SDL_CreateRenderer(left_window,-1,SDL_RENDERER_ACCELERATED);
	SDL_Renderer *right = SDL_CreateRenderer(right_window,-1,SDL_RENDERER_ACCELERATED);
	
	SDL_Event event;

	unsigned int score[2] = {0};
	score[0] = 2;
	score[1] = 8;
	//SDL_Color white = {255,255,255,255};
	SDL_Color gray = {64,64,64,255};

	TTF_Init();
	TTF_Font *font = TTF_OpenFont("Symtext.ttf", 80);	

	char left_label_buffor[2];
	sprintf(left_label_buffor,"%d",score[0]);
	left_label_buffor[1] = '\0';
	SDL_Surface *left_score_surface = TTF_RenderText_Blended(font,left_label_buffor,gray); 	
	SDL_Rect left_score_target = {WINDOW_WIDTH - 90, 0, left_score_surface->w, left_score_surface->h};
	SDL_Texture *left_score_texture = SDL_CreateTextureFromSurface(left,left_score_surface);
	SDL_FreeSurface(left_score_surface);

	char right_label_buffor[2];
	sprintf(right_label_buffor,"%d",score[1]);
	right_label_buffor[1] = '\0';
	SDL_Surface *right_score_surface = TTF_RenderText_Blended(font,right_label_buffor,gray); 	
	SDL_Rect right_score_target = {30, 0, right_score_surface->w, right_score_surface->h};
	SDL_Texture *right_score_texture = SDL_CreateTextureFromSurface(right,right_score_surface);
	SDL_FreeSurface(right_score_surface);

	bool running = true;

	
	while(running){
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			running = false;	
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_UP:
					if (player_1.y > 0) player_1.y--;;
					break;
				case SDLK_DOWN:
					if (player_1.y < WINDOW_HEIGHT - 50) player_1.y++;;
					break;
				default:
				;
				break;
			}
			break;
		default:
			;
			break;
	}

	SDL_SetRenderDrawColor(left,17,17,17,255);
	SDL_SetRenderDrawColor(right, 17,17,17,255);

	SDL_RenderClear(left);
	SDL_RenderClear(right);
	
	SDL_SetRenderDrawColor(left,255,255,255,255);
	SDL_SetRenderDrawColor(right,255,255,255,255);

	SDL_RenderFillRect(left, &player_1);
	SDL_RenderFillRect(right, &player_2);
	
	SDL_RenderCopy(left,left_score_texture, NULL, &left_score_target);
	SDL_RenderCopy(right,right_score_texture, NULL, &right_score_target);
	

	SDL_RenderFillRect(left, &left_ball);
	SDL_RenderFillRect(right, &right_ball);

	//SDL_SetWindowPosition(right_window,wp,100); 
	SDL_RenderPresent(left);
	SDL_RenderPresent(right);


	}
	TTF_Quit();
	SDL_Quit();

	return 0;
}
