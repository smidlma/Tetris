#ifndef TETRIS_H
#define TETRIS_H

#include <SDL.h>
#include "game.h"
#include <stdbool.h>

typedef struct {
    SDL_Point *pos;
    SDL_Color color;
    int shape_type;
} Block;

Block *curr_block;
static int blocks[7][4] = {
    1, 3, 5, 7,  // I
    0, 1, 2, 4,  // J
    0, 2, 4, 5,  // L
    0, 1, 2, 3,  // O
    0, 2, 3, 5,  // S
    1, 2, 3, 4,  // Z
    1, 2, 3, 5,  // T
};



Block *blockNew(int n);
SDL_Point *getShape(int n);
void drawBlock(Block *block, SDL_Renderer *rend);
void moveBlock(SDL_Point *block, int x, int y);
void rotateBlock(Block *block);

void saveBlockToGrid(int *grid);
void generateBlock();
void clearGrid(int *grid);
void printGrid(int *grid);
void drawGrid(int *grid, SDL_Renderer *rend);
void shiftGrid(int *grid, int n);

bool roatationCollision(SDL_Point *curr_block, int *grid);
bool collision(SDL_Point *curr_block, int *grid, int x, int y, bool *game_over);
bool lineCheck(int *grid);

#endif  // TETRIS_H