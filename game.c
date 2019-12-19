#include "game.h"

bool gameOver(SDL_Point *curr_block) {
    for (int i = 0; i < 4; i++) {
        if (curr_block[i].y < 0) {
            SDL_Log("Game Over");
            return true;
        }
    }

    return false;
}

void scoreAdd(int *score) {
    *score += 1;
}
Mix_Music *loadAudio(const char *file_path) {
    // Audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Music *music = Mix_LoadMUS(file_path);
    return music;
}
void playAudio(Mix_Music *music, int volume) {
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(music, -1);
}
void stopAudio() {
    // Mix_PauseMusic();
    Mix_FadeOutMusic(1000);
}
void drawText(SDL_Renderer *rend, const char *text, SDL_Rect dst_rect, SDL_Color color) {
    TTF_Font *font = TTF_OpenFont("resources/ARCADECLASSIC.TTF", dst_rect.h);

    SDL_Surface *surface_text = TTF_RenderText_Solid(font, text, color);

    SDL_Texture *title = SDL_CreateTextureFromSurface(rend, surface_text);

    SDL_RenderCopy(rend, title, NULL, &dst_rect);

    SDL_FreeSurface(surface_text);
    surface_text = NULL;

    SDL_DestroyTexture(title);
    title = NULL;

    TTF_CloseFont(font);
    font = NULL;
}
void drawMenu(SDL_Renderer *rend, Menu *menu) {
    SDL_SetRenderDrawColor(rend, 255, 255, 20, 255);

    for (int i = 0; i < 2; i++) {
        menu[i].button.w = WINDOW_WIDTH / 2;
        menu[i].button.h = WINDOW_HEIGHT / 8;
        menu[i].button.x = (WINDOW_WIDTH - menu[i].button.w) / 2;
        menu[i].button.y = ((WINDOW_HEIGHT) / 2) + ((menu[i].button.h + 20) * i);

        // SDL_RenderCopyEx(rend, texture, NULL, &menu[i], 0, NULL, false);
        SDL_RenderCopy(rend, menu[i].texture, NULL, &menu[i].button);
        //SDL_RenderFillRect(rend, &menu[i]);
    }
}
void drawTetrisBackground(SDL_Renderer *rend) {
    //  SDL_SetRenderDrawColor(rend, 20, 20, 20, 100);
    SDL_SetRenderDrawColor(rend, 96, 189, 184, 200);

    for (int y = 0; y < WINDOW_HEIGHT / SCL; y++) {
        for (int x = 0; x < WINDOW_WIDTH / SCL; x++) {
            SDL_RenderDrawLine(rend, x * SCL, 0, x * SCL, WINDOW_HEIGHT);
            SDL_RenderDrawLine(rend, 0, y * SCL, WINDOW_WIDTH, y * SCL);
        }
    }
}
void saveScore(const char *file_path, int score) {
    FILE *file = fopen(file_path, "w");
    if (!file) {
        SDL_Log("Can not open the file : %s", file_path);
    }
    fprintf(file, "%d", score);

    fclose(file);
    file = NULL;
}
int loadScore(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        SDL_Log("Can not open the file : %s", file_path);
    }
    int score = 0;
    fscanf(file, "%d", &score);

    fclose(file);
    file = NULL;

    return score;
}
