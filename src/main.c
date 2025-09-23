#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
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
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* screen;
    bool done;

    Tile levelTiles[LEVEL_WIDTH][LEVEL_HEIGHT];
    Player player;

} GameContext;

//functions
void init(GameContext* ctx) {
    //init
    SDL_Init(SDL_INIT_EVERYTHING);

    ctx->done = false;

    ctx->window = SDL_CreateWindow(
        WINDOW_TITLE, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * WINDOW_SCALE, 
        SCREEN_HEIGHT * WINDOW_SCALE, 
        0
    );
    ctx->renderer = SDL_CreateRenderer(ctx->window, 0, 0);
    ctx->screen = SDL_CreateTexture(
        ctx->renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
}
void handleEvents(GameContext* ctx) {
    SDL_Event event;
    while (SDL_PollEvent( &event )){
        if (event.type == SDL_QUIT) {
            ctx->done = true;
        }
        if (event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_SPACE)
                ctx->player.yVelocity = -200;
        }
    }
}
void update(GameContext* ctx, float deltaTime) {
    const Uint8* state = SDL_GetKeyboardState(NULL);

        printf("%f\n",ctx->player.yVelocity);

    ctx->player.yVelocity += 300 * deltaTime;


    if (state[SDL_SCANCODE_D]) {
        ctx->player.hitbox.x += 100 * deltaTime;
    }
    if (state[SDL_SCANCODE_A]) {
        ctx->player.hitbox.x -= 100 * deltaTime;
    }

    ctx->player.hitbox.y += ctx->player.yVelocity * deltaTime;
    //now check for collisions between plr and tiles.
    for (int tRow = 0; tRow < LEVEL_HEIGHT; tRow ++) {

        for (int tCol = 0; tCol < LEVEL_WIDTH; tCol ++) {

            if (ctx->levelTiles[tCol][tRow].id == 1) { // 1=solid/existing for now
                SDL_FRect tileRect = {
                    tCol*TILE_SIZE,
                    tRow*TILE_SIZE,
                    TILE_SIZE,TILE_SIZE
                };

                if (SDL_HasIntersectionF(
                    &ctx->player.hitbox, 
                    &tileRect
                )){
                    if ((tileRect.y-ctx->player.hitbox.y) > 0) {
                        ctx->player.hitbox.y = tileRect.y - ctx->player.hitbox.h;
                    } else{
                        ctx->player.hitbox.y = tileRect.y + tileRect.h;
                    }
                    ctx->player.yVelocity = 0;
                }
            }
            
        }

    }
}
void draw(GameContext* ctx) {
    SDL_SetRenderTarget(ctx->renderer, ctx->screen);

    SDL_SetRenderDrawColor(ctx->renderer, 127, 127, 127, 255);
    SDL_RenderClear(ctx->renderer);

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    for (int tRow = 0; tRow < LEVEL_HEIGHT; tRow ++) {

        for (int tCol = 0; tCol < LEVEL_WIDTH; tCol ++) {

            if (ctx->levelTiles[tCol][tRow].id == 1) { // 1=solid/existing for now
                SDL_RenderFillRect(
                    ctx->renderer, 
                    &(SDL_Rect){
                        tCol*TILE_SIZE,
                        tRow*TILE_SIZE,
                        TILE_SIZE,TILE_SIZE
                    }
                );
            }
            
        }

    }

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, 255);
    SDL_RenderFillRectF(ctx->renderer, &ctx->player.hitbox);

    SDL_RenderCopy(ctx->renderer, ctx->player.atlas, NULL, &(SDL_Rect){
        (int)ctx->player.hitbox.x,
        (int)ctx->player.hitbox.y,
        (int)ctx->player.hitbox.w,
        (int)ctx->player.hitbox.h
    });

    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_RenderCopy(ctx->renderer, ctx->screen, NULL, NULL);

    SDL_RenderPresent(ctx->renderer);
}
int main(int argc, char* args[]) {

    //this contains all necessary game components
    GameContext ctx;

    //init window, renderer, screen
    init(&ctx);

    //
    //beta

    //FIND A BETTER PLACE FOR THIS
    ctx.levelTiles[0][LEVEL_HEIGHT - 1].id = 1;
    ctx.levelTiles[1][LEVEL_HEIGHT - 1].id = 1;
    ctx.levelTiles[2][LEVEL_HEIGHT - 1].id = 1;
    ctx.levelTiles[3][LEVEL_HEIGHT - 1].id = 1;
    ctx.levelTiles[4][LEVEL_HEIGHT - 1].id = 1;

    ctx.levelTiles[6][LEVEL_HEIGHT - 3].id = 1;
    ctx.levelTiles[7][LEVEL_HEIGHT - 3].id = 1;
    ctx.levelTiles[8][LEVEL_HEIGHT - 3].id = 1;


    ctx.player = (Player){
        .hitbox = {0, 55, 16, 16}, 
        .atlas = IMG_LoadTexture(ctx.renderer, "res/john.png")
    };

    //game loop
    Uint32 frameStart;
    Uint32 frameElapsed;
    Uint32 deltaTime;

    while (!ctx.done) {
        frameStart = SDL_GetTicks();

        handleEvents(&ctx);
        update(&ctx, deltaTime / 1000.0f);
        draw(&ctx);

        frameElapsed = SDL_GetTicks() - frameStart;
        if (frameElapsed < FRAME_TARGET_TIME) {
            SDL_Delay(FRAME_TARGET_TIME - frameElapsed);
        }
        deltaTime = (SDL_GetTicks() - frameStart);// + frameElapsed;
    }

    return 0;
}