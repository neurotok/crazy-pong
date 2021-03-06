#include <stdio.h>
#include <SDL2/SDL.h>
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

int main(void)
{
	SDL_DisplayMode current;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GetCurrentDisplayMode(0, &current);

	//unsigned int wp = 100 + WINDOW_WIDTH + 50;

	unsigned int base_x = (current.w - (WINDOW_WIDTH * 2 + 100)) / 2;
	unsigned int base_y = (current.h - WINDOW_HEIGHT) / 2;
	SDL_Window *left_window = SDL_CreateWindow("Pong", base_x, base_y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_Window *right_window = SDL_CreateWindow("Pong", base_x + WINDOW_WIDTH + 100, base_y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);


	SDL_Rect player_1  = {20,WINDOW_HEIGHT / 2 - PAD_HEIGH / 2,PAD_WIGHT,PAD_HEIGH};
	SDL_Rect player_2 = {WINDOW_WIDTH - 30,WINDOW_HEIGHT / 2 - PAD_HEIGH / 2, PAD_WIGHT, PAD_HEIGH};


	float left_ball_x = WINDOW_WIDTH - 10;
	float left_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;

	float right_ball_x = -10.0f;
	float right_ball_y = left_ball_y;

	SDL_Rect left_ball= {(int)left_ball_x, (int)left_ball_y, BALL_SIZE, BALL_SIZE};
	SDL_Rect right_ball= {(int)right_ball_x, (int)right_ball_y, BALL_SIZE, BALL_SIZE};

	bool is_left = true;
	bool did_touch = false;
	
	float speed = 5.0f;
	float vector[2] = {-1.0f * speed, 0.0f * speed};

	SDL_Renderer *left = SDL_CreateRenderer(left_window,-1,SDL_RENDERER_ACCELERATED);
	SDL_Renderer *right = SDL_CreateRenderer(right_window,-1,SDL_RENDERER_ACCELERATED);

	//Digits target heneration
	SDL_Rect left_score_digit[15];
	SDL_Rect right_score_digit[15];

	for (int i = 0, n = 0; i < 15; ++i) {

		left_score_digit[i].x = WINDOW_WIDTH - BALL_SIZE * 3 - 20 + (BALL_SIZE * (i%3));
		right_score_digit[i].x = 20 + (BALL_SIZE * (i%3));
		left_score_digit[i].y = 20 + (BALL_SIZE * n);
		right_score_digit[i].y = 20 + (BALL_SIZE * n);

		left_score_digit[i].w = BALL_SIZE;
		right_score_digit[i].w = BALL_SIZE;
		left_score_digit[i].h = BALL_SIZE;
		right_score_digit[i].h = BALL_SIZE;

		if (i%3 == 2) {
			n++;
		}
	}

	SDL_Event event;

	unsigned int score[2] = {0};

	Uint32 last_time = 0;
	Uint32 delta_time = 0;
	Uint32 current_time; 
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
						if (player_1.y > 0) player_1.y-= (int)speed;;
						break;
					case SDLK_DOWN:
						if (player_1.y < WINDOW_HEIGHT - PAD_HEIGH) player_1.y += (int)speed;
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

		SDL_SetRenderDrawColor(left,92,92,92,255);
		SDL_SetRenderDrawColor(right,92,92,92,255);

		//Score rendering
		for (int i = 0; i < 15; ++i) {

			if (digits[score[0]][i]) {
				SDL_RenderFillRect(left,&(left_score_digit[i]));
			}
			if (digits[score[1]][i]) {
				SDL_RenderFillRect(right,&(right_score_digit[i]));
			}
		}

		SDL_SetRenderDrawColor(left,255,255,255,255);
		SDL_SetRenderDrawColor(right,255,255,255,255);

		//Ball movment 
		if (is_left) {
			left_ball_x += vector[0]; 
			left_ball_y += vector[1];
			right_ball_y += vector[1];
		}else{
			right_ball_x += vector[0];
			right_ball_y += vector[1];
			left_ball_y += vector[1];
		}
	
		///Ball target initialization & render
		left_ball.x = (int)left_ball_x;
		left_ball.y = (int)left_ball_y;
		SDL_RenderFillRect(left, &left_ball);

		right_ball.x = (int)right_ball_x;
		right_ball.y = (int)right_ball_y;
		SDL_RenderFillRect(right, &right_ball);
		
		SDL_RenderFillRect(left, &player_1);

		//Player 2 = ball position
		int player_2_y =  right_ball_y - PAD_HEIGH / 2;
		if (right_ball_y > PAD_HEIGH / 2 && right_ball_y < WINDOW_HEIGHT - PAD_HEIGH / 2) {
	
			if (player_2_y - player_2.y > 0){
				player_2.y++;
			}else{
				player_2.y--;
			}
		}

		SDL_RenderFillRect(right, &player_2);

		//Freeze left ball
		if(is_left == true && left_ball.x > WINDOW_WIDTH + BALL_SIZE){
			left_ball_x = WINDOW_WIDTH + BALL_SIZE;	
			is_left = !is_left;
			right_ball_x = -BALL_SIZE;
			did_touch = false;
			//printf("freeze\n");
		}
		//Freeze right ball
		if(is_left == false && right_ball.x < -BALL_SIZE){
			right_ball_x = -BALL_SIZE;;	
			is_left = !is_left;
			left_ball_x = WINDOW_WIDTH + BALL_SIZE;
			did_touch = false;
			//printf("freeze\n");
		}

		//Player bounce
		if (is_left == true && did_touch == false && left_ball.y + BALL_SIZE > player_1.y && left_ball.y  < player_1.y + PAD_HEIGH && left_ball.x < 20 + PAD_WIGHT) {
			int d = (left_ball.y + BALL_SIZE / 2) - (player_1.y + PAD_HEIGH / 2);
			vector[0] = -vector[0];
			vector[1] += d * 0.5 / (PAD_HEIGH / 2) * speed;
			did_touch = true;
			//printf("hit\n");

		}
		if(is_left == false && did_touch == false && right_ball.y + BALL_SIZE > player_2.y && right_ball.y < player_2.y + PAD_HEIGH && right_ball.x > WINDOW_WIDTH - 40){

			int d = (right_ball.y + BALL_SIZE / 2) - (player_2.y + PAD_HEIGH / 2);
			vector[0] = -vector[0];
			vector[1] += d * 0.5 / (PAD_HEIGH / 2) * speed;
			did_touch = true;
			//printf("hit\n");
		}	

		//Bottom || top edge bounce
		if (right_ball.y < 0 || right_ball.y + BALL_SIZE > WINDOW_HEIGHT || left_ball.y < 0 || left_ball.y + BALL_SIZE > WINDOW_HEIGHT) {

			vector[1] = -vector[1];
			//printf("bounce\n");

		}

		//Score
		if (is_left == true && did_touch  == false && left_ball.x + 10 < 0) {

			if (score[1] < 9) {
				score[1]++;

			}	
			else{
				score[0] = 0;
				score[1] = 0;
			}
			left_ball_x = WINDOW_WIDTH - BALL_SIZE;
			left_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
			right_ball_x = -10;
			right_ball_y = left_ball_y;
			vector[0] = -1.0f * speed;
			vector[1] = 0.0f;
			is_left = true;
			//printf("score\n");

		}
		if (is_left == false  && did_touch == false && right_ball.x  > WINDOW_WIDTH) {

			if (score[0] < 9) {
				score[0]++;

			}	
			else{

				score[0] = 0;
				score[1] = 0;

			}
			left_ball_x = WINDOW_WIDTH + BALL_SIZE;
			left_ball_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
			right_ball_x = -10;
			right_ball_y = left_ball_y;
			vector[0] = -1.0f * speed;
			vector[1] = 0.0f;
			is_left = true;
			//printf("score\n");
		}

		current_time = SDL_GetTicks();
		delta_time = current_time - last_time;
		last_time = current_time;

		if (delta_time < 1000/60) {
			SDL_Delay(1000/60 - delta_time);
		}

		/*
		unsigned int left_win_x = WINDOW_WIDTH / 2 + left_ball_x / WINDOW_WIDTH * 50;
		unsigned int left_win_y = left_ball_x / WINDOW_WIDTH * 50;
		
		SDL_SetWindowPosition(left_window, base_x + left_win_x,base_y + left_win_y); 
		*/	
	
		SDL_RenderPresent(left);
		SDL_RenderPresent(right);
	

	}
	SDL_Quit();

	return 0;
}


