#include <SDL.h>
#include <SDL_main.h>
#include "game.h"

int main(int argc, char* args[]) {
    //this contains all necessary game components
    GameContext ctx = {0};

    //init window, renderer, screen
    Game_Init(&ctx);

    //game loop
    Uint32 frameStart = SDL_GetTicks();
    Uint32 frameElapsed;
    float deltaTime;

    while (!ctx.done) {
        deltaTime = (SDL_GetTicks() - frameStart) / 1000.0f;
        frameStart = SDL_GetTicks();

        Game_HandleEvents(&ctx);
        Game_Update(&ctx, deltaTime);
        Game_Render(&ctx);

        frameElapsed = SDL_GetTicks() - frameStart;
        if (frameElapsed < FRAME_TARGET_TIME) {
            SDL_Delay(FRAME_TARGET_TIME - frameElapsed);
        }
    }

    return 0;
}