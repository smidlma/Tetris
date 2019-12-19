#include "tetris.h"

Block *blockNew(int n) {
    Block *block = (Block *)malloc(sizeof(Block));
    SDL_Color c = {255, 0, 0, 255};
    block->pos = getShape(n);
    block->shape_type = n;
    block->color = c;

    return block;
}

SDL_Point *getShape(int n) {
    SDL_Point *pos = (SDL_Point *)malloc(4 * sizeof(SDL_Point));
    for (int i = 0; i < 4; i++) {
        pos[i].x = (blocks[n][i] % 2);
        pos[i].y = (blocks[n][i] / 2);
    }
    return pos;
}

void drawBlock(Block *block, SDL_Renderer *rend) {
    // SDL_SetRenderDrawColor(rend, block->color.r, block->color.g, block->color.b, block->color.a);
    SDL_Texture *texture = IMG_LoadTexture(rend, "resources/blue_block.png");

    for (int i = 0; i < 4; i++) {
        SDL_Rect rect;
        rect.x = block->pos[i].x * SCL;
        rect.y = block->pos[i].y * SCL;
        rect.w = SCL;
        rect.h = SCL;
        SDL_RenderCopy(rend, texture, NULL, &rect);

        // SDL_RenderFillRect(rend, &rect);
    }
}

void moveBlock(SDL_Point *block, int x, int y) {
    for (int i = 0; i < 4; i++) {
        block[i].x += x;
        block[i].y += y;
    }
}
void rotateBlock(Block *block) {
    if (block->shape_type == 3) return;
    SDL_Point pivot;

    pivot.x = block->pos[1].x;
    pivot.y = block->pos[1].y;

    for (int i = 0; i < 4; i++) {
        SDL_Point v_r;

        v_r.x = block->pos[i].x - pivot.x;
        v_r.y = block->pos[i].y - pivot.y;
        int x = v_r.y * -1;
        int y = 1 * v_r.x;

        block->pos[i].x = pivot.x + x;
        block->pos[i].y = pivot.y + y;
    }
}
bool collision(SDL_Point *curr_block, int *grid, int x, int y, bool *game_over) {
    for (int i = 0; i < 4; i++) {
        if (curr_block[i].y >= 0) {
            // Check possition before applying rotation
            if (x == 0 && y == 0) {
                return roatationCollision(curr_block, grid);
            }
            if (curr_block[i].y + y == WINDOW_HEIGHT / SCL ||
                grid[(curr_block[i].x) + (curr_block[i].y + y) * (WINDOW_WIDTH / SCL)] == 1) {
                // Over edge ? u lost
                *game_over = gameOver(curr_block);

                if (!*game_over) {
                    saveBlockToGrid(grid);
                    //printGrid(grid);
                    generateBlock();
                }

                return true;
            }
            if (curr_block[i].x + x < 0 || curr_block[i].x + x == WINDOW_WIDTH / SCL ||
                grid[(curr_block[i].x + x) + curr_block[i].y * (WINDOW_WIDTH / SCL)] == 1) {
                return true;
            }
        }
    }
    return false;
}

void saveBlockToGrid(int *grid) {
    for (int i = 0; i < 4; i++) {
        int x = curr_block->pos[i].x;
        int y = curr_block->pos[i].y;

        grid[x + y * (WINDOW_WIDTH / SCL)] = 1;
    }
}

void generateBlock() {
    int shape_type = rand() % 7 + 0;

    curr_block = blockNew(shape_type);

    // Starting poss of block
    int x = rand() % ((WINDOW_WIDTH / SCL - 5) - 4 ) + 5;  // (WINDOW_WIDTH / SCL / 2) - 1
    moveBlock(curr_block->pos, x, -4);
}

void clearGrid(int *grid) {
    for (int y = 0; y < WINDOW_HEIGHT / SCL; y++) {
        for (int x = 0; x < WINDOW_WIDTH / SCL; x++) {
            grid[x + y * (WINDOW_WIDTH / SCL)] = 0;
        }
    }
}

void printGrid(int *grid) {
    SDL_Log("Grid:");

    for (int y = 0; y < (WINDOW_HEIGHT / SCL); y++) {
        for (int x = 0; x < (WINDOW_WIDTH / SCL); x++) {
            printf("%d", grid[x + y * (WINDOW_WIDTH / SCL)]);
        }
        printf("\n");
    }
}

void drawGrid(int *grid, SDL_Renderer *rend) {
    SDL_Texture *texture = IMG_LoadTexture(rend, "resources/orange_block.png");
    for (int y = 0; y < (WINDOW_HEIGHT / SCL); y++) {
        for (int x = 0; x < (WINDOW_WIDTH / SCL); x++) {
            if (grid[x + y * (WINDOW_WIDTH / SCL)] == 1) {
                SDL_Rect rect;
                rect.x = x * SCL;
                rect.y = y * SCL;
                rect.w = SCL;
                rect.h = SCL;
                SDL_RenderCopy(rend, texture, NULL, &rect);
                // SDL_RenderFillRect(rend, &rect);
            }
        }
    }
}
void shiftGrid(int *grid, int n) {
    printGrid(grid);

    //int count = (WINDOW_HEIGHT / SCL) - y
    for (int y = n; y > 0; y--) {
        for (int x = 0; x < WINDOW_WIDTH / SCL; x++) {
            grid[x + y * WINDOW_WIDTH / SCL] = grid[x + (y - 1) * WINDOW_WIDTH / SCL];
        }
    }

    printGrid(grid);
}

bool lineCheck(int *grid) {
    for (int y = 0; y < WINDOW_HEIGHT / SCL; y++) {
        int count = 0;
        for (int x = 0; x < WINDOW_WIDTH / SCL; x++) {
            count += grid[x + y * WINDOW_WIDTH / SCL];
            if (count == WINDOW_WIDTH / SCL) {
                shiftGrid(grid, y);
                return true;
            }
        }
    }
    return false;
}

bool roatationCollision(SDL_Point *curr_block, int *grid) {
    SDL_Point pivot;

    pivot.x = curr_block[1].x;
    pivot.y = curr_block[1].y;

    for (int i = 0; i < 4; i++) {
        SDL_Point v_r;

        v_r.x = curr_block[i].x - pivot.x;
        v_r.y = curr_block[i].y - pivot.y;
        int x = v_r.y * -1;
        int y = 1 * v_r.x;

        int x_r = pivot.x + x;
        int y_r = pivot.y + y;

        if (x_r < 0 || x_r >= WINDOW_WIDTH / SCL || y_r >= WINDOW_HEIGHT/SCL) {
            return true;
        }

        for (int y = 0; y < WINDOW_HEIGHT / SCL; y++) {
            for (int x = 0; x < WINDOW_WIDTH / SCL; x++) {
                if (grid[x_r + y_r * WINDOW_WIDTH / SCL] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}
