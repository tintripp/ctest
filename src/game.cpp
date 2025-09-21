#include "game.h"

Game::Game() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        done = true;
    }

    // Create window and renderer
    if (SDL_CreateWindowAndRenderer("sigma", 640, 480, NULL, &window, &renderer) < 0) {
        SDL_Log("Window and renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        done = true;
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (!done) {
        handleEvents(&event);
        update();
        draw();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void Game::handleEvents(SDL_Event *event) {
    while (SDL_PollEvent(event)){
        if (event->type == SDL_EVENT_QUIT){
            done = true;
        }
    }
}

void Game::update() {

}

void Game::draw() {

}