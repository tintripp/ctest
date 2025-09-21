#pragma once
#include <SDL3/SDL.h>
#include "texman.h"

class Game {
private:
    bool done = false;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *screen = NULL;

    TextureManager texman;

    void handleEvents();
    void update();
    void draw();
public:
    Game();
};