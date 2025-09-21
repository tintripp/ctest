#pragma once
#include <SDL3/SDL.h>

class Game {
private:
    bool done = false;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    void handleEvents(SDL_Event* event);
    void update();
    void draw();
public:
    Game();
};