#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <math.h>

#include "digits.h"

#define PI (3.14159265358979323846)  

enum {
	WINDOW_WIDTH = 350,
	WINDOW_HEIGHT = 500,
	PAD_HEIGH = 100,
	PAD_WIGHT = 10,
	BALL_SIZE = 10,
	MAX_ANGLE = 20
};

//SDL_Color white = {255,255,255,255};
SDL_Color gray = {64,64,64,255};

void rebuldLabelTexture(SDL_Renderer *render_target, SDL_Texture *label_texture,TTF_Font *font, unsigned int score){

	char buffor[2];
	sprintf(buffor,"%d",score);
	buffor[1] = '\0';
	SDL_Surface *score_surface = TTF_RenderText_Blended(font,buffor,gray); 	
	SDL_DestroyTexture(label_texture);
	label_texture = SDL_CreateTextureFromSurface(render_target,score_surface);
	SDL_FreeSurface(score_surface);

}

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


	SDL_Rect player_1  = {20,WINDOW_HEIGHT / 2 - PAD_HEIGH / 2,PAD_WIGHT,PAD_HEIGH};
	SDL_Rect player_2 = {WINDOW_WIDTH - 30,WINDOW_HEIGHT / 2 - PAD_HEIGH / 2, PAD_WIGHT, PAD_HEIGH};


	float left_ball_x = WINDOW_WIDTH - 10;
	float left_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;

	float right_ball_x = -10.0f;
	float right_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;

	SDL_Rect left_ball= {(int)left_ball_x, (int)left_ball_y, BALL_SIZE, BALL_SIZE};
	SDL_Rect right_ball= {(int)right_ball_x, (int)right_ball_y, BALL_SIZE, BALL_SIZE};

	bool is_left = true;
	float vector[2] = {-1.0,0.0};


	SDL_Renderer *left = SDL_CreateRenderer(left_window,-1,SDL_RENDERER_ACCELERATED);
	SDL_Renderer *right = SDL_CreateRenderer(right_window,-1,SDL_RENDERER_ACCELERATED);

	SDL_Event event;

	unsigned int score[2] = {0};


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

	unsigned int base_x = 50;
	unsigned int base_y = 50;

	SDL_Rect left_score_digit[15];

	for (int i = 0; i < 15; ++i) {
		left_score_digit[i].x = base_x + (BALL_SIZE * i);
		left_score_digit[i].y = base_y + (BALL_SIZE * i);
		left_score_digit[i].w = BALL_SIZE;
		left_score_digit[i].h = BALL_SIZE;
	}

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

		for (int i = 0; i < 15; ++i) {
			SDL_RenderFillRect(left,&(left_score_digit[i]));
		}
		
	
		//Ball movment
		if (is_left) {
			left_ball_x += vector[0];
			left_ball_y += vector[1];
		}else{
			right_ball_x += vector[0];
			right_ball_y += vector[1];
		}
		
		left_ball.x = (int)left_ball_x;
		left_ball.y = (int)left_ball_y;
		SDL_RenderFillRect(left, &left_ball);

		right_ball.x = (int)right_ball_x;
		right_ball.y = (int)right_ball_y;
		SDL_RenderFillRect(right, &right_ball);

		//Freeze left ball
		if(is_left == true && left_ball.x > WINDOW_WIDTH + BALL_SIZE){
			printf("Left ball freeze\n");
			left_ball.x = WINDOW_WIDTH + BALL_SIZE;	
			is_left = !is_left;
			right_ball.x = -BALL_SIZE;
			right_ball.y = left_ball.y;
		}
		//Freeze right ball
		if(is_left == false && right_ball.x < -BALL_SIZE){
			printf("RIght ball freeze\n");
			right_ball.x = -BALL_SIZE;;	
			is_left = !is_left;
			left_ball.x = WINDOW_WIDTH + BALL_SIZE;
			left_ball.y = right_ball.y;
		}

		//Player bounce
		if (is_left == true && left_ball.y + BALL_SIZE > player_1.y && left_ball.y  < player_1.y + PAD_HEIGH && left_ball.x < 30) {
			int d = (left_ball.y + BALL_SIZE / 2) - (player_1.y + PAD_HEIGH / 2);

			printf("Left bounce at %d\n",d);
			vector[0] = -vector[0];
			vector[1] = -vector[1];


		}
		if(is_left == false && right_ball.y + BALL_SIZE > player_2.y && right_ball.y < player_2.y + PAD_HEIGH && right_ball.x > WINDOW_WIDTH - 40){

			int d = (right_ball.y + BALL_SIZE / 2) - (player_2.y + PAD_HEIGH / 2);
			printf("Right bounce at %d\n", d);
			vector[0] = -vector[0];
			vector[1] = -vector[1];
		}	
		//Bottom || top edge bounce
		if (right_ball.y < 0 || right_ball.y + BALL_SIZE > WINDOW_HEIGHT || left_ball.y < 0 || left_ball.y + BALL_SIZE > WINDOW_HEIGHT) {
			vector[0] = vector[0];
			vector[1] = -vector[1];

		}


		//Score
		if (is_left == true && left_ball.x + 10 < 0) {
			if (score[1] < 10) {
				score[1]++;
				
				rebuldLabelTexture(right, right_score_texture, font, score[1]);
			}	
			else{
				score[0] = 0;
				score[1] = 0;

				rebuldLabelTexture(left, left_score_texture, font, score[0]);
				rebuldLabelTexture(right, right_score_texture, font, score[1]);
			}

			left_ball_x = WINDOW_WIDTH - BALL_SIZE;
			left_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
			right_ball_x = -10;
			right_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
			vector[0] = -1.0f;
			vector[1] = 0.0f;

		}
		if (is_left == false && right_ball.x  > WINDOW_WIDTH) {
			if (score[0] < 10) {
				score[0]++;

				rebuldLabelTexture(left, left_score_texture, font, score[0]);
			}	
			else{
				score[0] = 0;
				score[1] = 0;

				rebuldLabelTexture(left, left_score_texture, font, score[0]);
				rebuldLabelTexture(right, right_score_texture, font, score[1]);
			}
			left_ball_x = WINDOW_WIDTH + BALL_SIZE;
			left_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
			right_ball.x = -10;
			right_ball.y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
			vector[0] = -1.0f;
			vector[1] = 0.0f;

		}
		



		//SDL_SetWindowPosition(right_window,wp,100); 
		SDL_RenderPresent(left);
		SDL_RenderPresent(right);


	}
	TTF_Quit();
	SDL_Quit();

	return 0;
}


