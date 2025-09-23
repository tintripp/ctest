#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_main.h>
#include "constants.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 15

//structs
typedef struct {
    Uint8 id;
} Tile;

typedef struct {
    SDL_Texture* atlas;

    SDL_FRect hitbox; //position + size.
    float xVelocity;
    float yVelocity;
} Player;

//global variables
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* screen;
SDL_Event e;
bool done = false;

//beta

//FIND A BETTER PLACE FOR THIS
Tile levelTiles[LEVEL_WIDTH][LEVEL_HEIGHT];

SDL_FRect player = {0, 0, 16, 16};

//functions
void handleEvents() {
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT) {
            done = true;
        }
    }
}
void update(float deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) {
        player.x += 100 * deltaTime;
    }
    if (state[SDL_SCANCODE_SPACE]) {
        player.x -= 100 * deltaTime;
    }
}
void draw() {
    SDL_SetRenderTarget(renderer, screen);

    SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int tRow = 0; tRow < LEVEL_HEIGHT; tRow ++) {
        for (int tCol = 0; tCol < LEVEL_WIDTH; tCol ++) {
            if (levelTiles[tCol][tRow].id == 1) { // solid/existing for now
                SDL_RenderFillRect(
                    renderer, 
                    &(SDL_Rect){
                        tCol*TILE_SIZE,
                        tRow*TILE_SIZE,
                        TILE_SIZE,TILE_SIZE
                    }
                );
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRectF(renderer, &player);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, screen, NULL, NULL);

    SDL_RenderPresent(renderer);
}
int main(int argc, char* args[]) {
    //init
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        WINDOW_TITLE, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * WINDOW_SCALE, 
        SCREEN_HEIGHT * WINDOW_SCALE, 
        0
    );
    renderer = SDL_CreateRenderer(window, 0, 0);
    screen = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        SCREEN_WIDTH, SCREEN_HEIGHT
    );

    Uint32 frameStart;
    Uint32 frameElapsed;
    Uint32 deltaTime;

    //beta
    levelTiles[1][0].id = 1;
    levelTiles[2][0].id = 1;
    levelTiles[3][0].id = 1;
    levelTiles[4][0].id = 1;
    levelTiles[5][0].id = 1;

    while (!done) {
        frameStart = SDL_GetTicks();

        handleEvents();
        update(deltaTime / 1000.0f);
        draw();

        frameElapsed = SDL_GetTicks() - frameStart;
        if (frameElapsed < FRAME_TARGET_TIME) {
            SDL_Delay(FRAME_TARGET_TIME - frameElapsed);
        }
        deltaTime = frameElapsed + (SDL_GetTicks() - frameStart);
    }

    return 0;
}