#include "game.h"
#include "constants.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>

Game::Game() {
    bool success = SDL_CreateWindowAndRenderer(
        WINDOW_TITLE, 
        SCREEN_WIDTH * WINDOW_SCALEFACTOR, 
        SCREEN_HEIGHT * WINDOW_SCALEFACTOR, 
        NULL, &window, &renderer
    );
    if (!success) {
        SDL_Log("Window and renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        done = true;
    }

    screen = SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET,
        SCREEN_WIDTH, 
        SCREEN_HEIGHT
    );
    SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);
    
    texman.set(renderer);
    texman.add("john.png");

    //draw on pixel perfect texture
    SDL_SetRenderTarget(renderer, screen);
    SDL_RenderClear(renderer);
    SDL_FRect dstRect;
    dstRect.w = 16;
    dstRect.h = 16;
    SDL_RenderTexture(renderer, texman["john.png"], NULL, &dstRect);
    //go back to screen boi
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderTexture(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);

    while (!done) {
        handleEvents();
        update();
        draw();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            done = true;
        }
    }
}

void Game::update() {

}

void Game::draw() {

}