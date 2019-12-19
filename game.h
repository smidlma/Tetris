#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define SCL 30
#define WINDOW_WIDTH (12 * SCL)
#define WINDOW_HEIGHT (20 * SCL)
#define SCORE_FILE "resources/score.txt"

typedef struct 
{
    SDL_Rect button;
    SDL_Texture *texture;
}Menu;

bool gameOver(SDL_Point *curr_block);

Mix_Music *loadAudio(const char *file_path);
void playAudio(Mix_Music *music, int volume);
void stopAudio();
void scoreAdd(int *score);
void drawText(SDL_Renderer *rend, const char *text, SDL_Rect dst_rect, SDL_Color color);
void drawMenu(SDL_Renderer *rend, Menu *menu);
void drawTetrisBackground(SDL_Renderer *rend);
void saveScore(const char *file_path, int score);
int loadScore(const char *file_path);

#endif