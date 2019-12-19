#include "game.h"
#include "tetris.h"

bool close;      // close game
bool game_over;  // player lost

void checkUserInput(SDL_Event event, int *grid);

bool buttonClicked(SDL_Rect rect, int x, int y);

int main() {
    srand(time(NULL));
    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window *win = SDL_CreateWindow("Tetris Game",  // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // triggers the program that controls

    // creates a renderer to render our images
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (TTF_Init() != 0) {
        printf("error initializing SDL TTF: %s\n", SDL_GetError());
    }

    // SETUP --------------------------------------------

    SDL_Texture *play_btn_texture = NULL;
    SDL_Texture *exit_btn_texture = NULL;
    play_btn_texture = IMG_LoadTexture(rend, "resources/arcadeplaybutton.png");
    exit_btn_texture = IMG_LoadTexture(rend, "resources/arcadeexitbutton.png");

    // Mix_Music *music = loadAudio("resources/Thomas.mp3");
    Mix_Music *music = loadAudio("resources/Dont_stop_the_music.mp3");

    game_over = true;
    close = false;

    // Menu Title
    SDL_Rect title_rect;
    title_rect.h = WINDOW_HEIGHT / 5;
    title_rect.w = WINDOW_WIDTH - 40;
    title_rect.x = (WINDOW_WIDTH - title_rect.w) / 2;
    title_rect.y = 30;

    SDL_Color title_color = {255, 255, 255, 255};

    // Best score
    SDL_Rect best_score_rect;
    best_score_rect.h = WINDOW_HEIGHT / 8;
    best_score_rect.w = WINDOW_WIDTH - 40;
    best_score_rect.x = (WINDOW_WIDTH - best_score_rect.w) / 2;
    best_score_rect.y = title_rect.h + title_rect.y + 20;
    int best_score = loadScore(SCORE_FILE);
    char best_score_text[35];

    sprintf(best_score_text, "BEST SCORE  %d", best_score);

    // Score counter
    SDL_Rect score_rect;
    score_rect.h = WINDOW_HEIGHT / 8;
    score_rect.w = WINDOW_WIDTH / 8;
    score_rect.x = (WINDOW_WIDTH - score_rect.w) / 2;
    score_rect.y = 5;

    // Menu buttons

    Menu *menu = (Menu *)malloc(sizeof(Menu) * 2);
    menu[0].texture = play_btn_texture;
    menu[1].texture = exit_btn_texture;

    int timer = 0;

    int score = 0;
    char score_text[10];
    sprintf(score_text, "%d", score);

    int speed = 20;
    int *grid = (int *)malloc(sizeof(int) * (WINDOW_HEIGHT / SCL) * (WINDOW_WIDTH / SCL));

    // Mouse
    int x, y;
    // GAME LOOP
    while (!close) {
        SDL_Event event;
        // User inputs -----------------------------------
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button) {
                        case SDL_BUTTON_RIGHT:
                                game_over = true;
                            break;
                        case SDL_BUTTON_LEFT:

                            SDL_GetMouseState(&x, &y);

                            if (buttonClicked(menu[0].button, x, y) && game_over) {
                                SDL_Log("Button clicked: Play");
                                clearGrid(grid);
                                generateBlock();
                                game_over = false;
                                score = 0;
                                playAudio(music, 20);
                            }

                            if (buttonClicked(menu[1].button, x, y) && game_over) {
                                SDL_Log("Button clicked: Close");
                                close = true;
                            }
                            break;

                        default:
                            break;
                    }
            }
            if (!game_over) {
                checkUserInput(event, grid);
            }
        }

        // UPDATE ----------------------------------------
        if (!game_over) {
            if (timer > speed && !collision(curr_block->pos, grid, 0, 1, &game_over)) {
                moveBlock(curr_block->pos, 0, 1);
                timer = 0;
            }
            timer++;
            if (lineCheck(grid)) {
                scoreAdd(&score);
                sprintf(score_text, "%d", score);
                SDL_Log("score: %d", score);
            }
        } else if (score > best_score) {
            saveScore(SCORE_FILE, score);
            best_score = loadScore(SCORE_FILE);
        }

        // DRAW ------------------------------------------

        if (!game_over) {
            // Background
            SDL_SetRenderDrawColor(rend, 4, 21, 31, 255);
            // SDL_SetRenderDrawColor(rend, timer*10,timer*10,timer*10, 255);

            SDL_RenderClear(rend);

            // Draw block
            drawBlock(curr_block, rend);
            // Draw tetris grid
            drawGrid(grid, rend);
            // Draw score
            drawText(rend, score_text, score_rect, title_color);
            drawTetrisBackground(rend);

        } else {
            // Background

            SDL_SetRenderDrawColor(rend, 4, 21, 31, 255);
            SDL_RenderClear(rend);
            drawTetrisBackground(rend);

            // Draw menu
            drawText(rend, "Tetris", title_rect, title_color);
            sprintf(best_score_text, "BEST SCORE  %d", best_score);

            drawText(rend, best_score_text, best_score_rect, title_color);  // load

            drawMenu(rend, menu);

            stopAudio();
            score = 0;
        }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }

    Mix_FreeMusic(music);

    SDL_DestroyTexture(play_btn_texture);
    SDL_DestroyTexture(exit_btn_texture);

    // dectroy renderer
    SDL_DestroyRenderer(rend);

    // dectroy window
    SDL_DestroyWindow(win);

    free(curr_block);
    free(grid);
    curr_block = NULL;
    grid = NULL;

    return 0;
}

bool buttonClicked(SDL_Rect rect, int x, int y) {
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) {
        return true;
    }
    return false;
}

void checkUserInput(SDL_Event event, int *grid) {
    switch (event.type) {
        case SDL_KEYDOWN:
            // keyboard API for key pressed
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    if (!collision(curr_block->pos, grid, -1, 0, &game_over)) {
                        moveBlock(curr_block->pos, -1, 0);
                    }
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    if (!collision(curr_block->pos, grid, 1, 0, &game_over)) {
                        moveBlock(curr_block->pos, 1, 0);
                    }
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if (!collision(curr_block->pos, grid, 0, 1, &game_over)) {
                        moveBlock(curr_block->pos, 0, 1);
                    }
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    if (!collision(curr_block->pos, grid, 0, 0, &game_over)) {  // 0 0 for rotation
                        rotateBlock(curr_block);
                    }
                    break;
            }
    }
}
