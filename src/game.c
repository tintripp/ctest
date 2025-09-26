#include "game.h"
#include <SDL.h>
#include <SDL_image.h>
#include "ecs.h"

//functions
void Game_Init(GameContext* ctx) {
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

    ////////////////////

    //FIND A BETTER PLACE FOR THIS 
    //

    //OBVIOUSLY will be in level data, hahahaha.
    ctx->levelTiles[0][LEVEL_HEIGHT - 1] = 1;
    ctx->levelTiles[1][LEVEL_HEIGHT - 1] = 1;
    ctx->levelTiles[2][LEVEL_HEIGHT - 1] = 1;
    ctx->levelTiles[3][LEVEL_HEIGHT - 1] = 1;
    ctx->levelTiles[4][LEVEL_HEIGHT - 1] = 1;

    ctx->levelTiles[6][LEVEL_HEIGHT - 3] = 1;
    ctx->levelTiles[7][LEVEL_HEIGHT - 3] = 1;
    ctx->levelTiles[8][LEVEL_HEIGHT - 3] = 1;


    //ECS TEST!

    Entity player = createEntity();

    NEW_COMPONENT(ctx->ecs.player, player, Player, 0);
    ctx->ecs.player[player]->jumpHeightMax = -200;
    ctx->ecs.player[player]->jumpHeightMin = -50;

    NEW_COMPONENT(ctx->ecs.velocity, player, SDL_FPoint, 0);
    NEW_COMPONENT(ctx->ecs.texture, player, SDL_Texture*, 
        IMG_LoadTexture(ctx->renderer, "res/john.png")
    );

    NEW_COMPONENT(ctx->ecs.hitbox, player, SDL_FRect, 
        .x = 0, 
        .y = 55, 
        .w = 16, 
        .h = 16
    );
    NEW_COMPONENT(ctx->ecs.gravity, player, Gravity, 
        .up = 300, 
        .down = 900
    );
}
void Game_HandleEvents(GameContext* ctx) {
    SDL_Event event;
    while (SDL_PollEvent( &event )){
        if (event.type == SDL_QUIT) {
            ctx->done = true;
        }

        for (int e = 0; e < MAX_ENTITIES; e ++) {
            if (ctx->ecs.player[e]) {
                Player* player = ctx->ecs.player[e];
                SDL_FPoint* velocity = ctx->ecs.velocity[e];
                
                if (event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.sym == SDLK_SPACE && player->onGround) {
                        velocity->y = player->jumpHeightMax;
                        player->jumpCancelled = false;
                    }
                }
                if (event.type == SDL_KEYUP) {
                    if(event.key.keysym.sym == SDLK_SPACE && velocity->y <= player->jumpHeightMin) {
                        velocity->y = player->jumpHeightMin;
                        player->jumpCancelled = true;
                    }
                }

            }
        }

    }
}
void Game_Update(GameContext* ctx, float deltaTime) {
    for (int e = 0; e < MAX_ENTITIES; e ++) {
        //everything past this point requires velocity
        if (!ctx->ecs.velocity[e]) continue;

        if (ctx->ecs.player[e]) {
            const Uint8* state = SDL_GetKeyboardState(NULL);

            ctx->ecs.velocity[e]->x = 0;
            if (state[SDL_SCANCODE_D]) {
                ctx->ecs.velocity[e]->x = 100;
            }
            if (state[SDL_SCANCODE_A]) {
                ctx->ecs.velocity[e]->x = -100;
            }            
        }

        //GRAVITY
        if (ctx->ecs.gravity[e]) {

            bool playerCancelled = false;
            if (ctx->ecs.player[e])
                playerCancelled = ctx->ecs.player[e]->jumpCancelled;

            float gravity = ctx->ecs.gravity[e]->up;
            if (ctx->ecs.velocity[e]->y > 0 || playerCancelled)
                gravity = ctx->ecs.gravity[e]->down;
        
            ctx->ecs.velocity[e]->y += gravity * deltaTime;

        }

        //COLLISION

        //if hitbox
        if (ctx->ecs.hitbox[e]) {
            SDL_FRect* myHitbox = ctx->ecs.hitbox[e];
            SDL_FPoint* myVelocity = ctx->ecs.velocity[e];

            //apply x velocity
            myHitbox->x += myVelocity->x * deltaTime;
            //push out x
            for (int tRow = 0; tRow < LEVEL_HEIGHT; tRow ++) {
                for (int tCol = 0; tCol < LEVEL_WIDTH; tCol ++) {

                    if (ctx->levelTiles[tCol][tRow] == 1) { // 1=solid/existing for now
                        SDL_FRect tileRect = {
                            tCol*TILE_SIZE,
                            tRow*TILE_SIZE,
                            TILE_SIZE,TILE_SIZE
                        };

                        if (SDL_HasIntersectionF(
                            ctx->ecs.hitbox[e], 
                            &tileRect
                        )){
                            if ((tileRect.x - myHitbox->x) > 0) {
                                myHitbox->x = tileRect.x - myHitbox->w;
                            } else{
                                myHitbox->x = tileRect.x + tileRect.w;
                            }
                            myVelocity->x = 0;
                        }
                    }
                    
                }
            }

            //apply y velocity
            myHitbox->y += myVelocity->y * deltaTime;

            //reset onGround flag for players every frame
            if (ctx->ecs.player[e])
                ctx->ecs.player[e]->onGround = false;

            for (int tRow = 0; tRow < LEVEL_HEIGHT; tRow ++) {
                for (int tCol = 0; tCol < LEVEL_WIDTH; tCol ++) {

                    if (ctx->levelTiles[tCol][tRow] == 1) { // 1=solid/existing for now
                        SDL_FRect tileRect = {
                            tCol*TILE_SIZE,
                            tRow*TILE_SIZE,
                            TILE_SIZE,TILE_SIZE
                        };

                        if (SDL_HasIntersectionF(
                            ctx->ecs.hitbox[e], 
                            &tileRect
                        )){
                            if ((tileRect.y - myHitbox->y) > 0) {
                                myHitbox->y = tileRect.y - myHitbox->h;

                                if (ctx->ecs.player[e])
                                    ctx->ecs.player[e]->onGround = true;
                            } else{
                                myHitbox->y = tileRect.y + tileRect.h;
                            }
                            myVelocity->y = 0;
                        }
                    }
                    
                }
            }

        }
    }


    
}
void Game_Render(GameContext* ctx) {
    SDL_SetRenderTarget(ctx->renderer, ctx->screen);

    SDL_SetRenderDrawColor(ctx->renderer, 127, 127, 127, 255);
    SDL_RenderClear(ctx->renderer);

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    for (int tRow = 0; tRow < LEVEL_HEIGHT; tRow ++) {

        for (int tCol = 0; tCol < LEVEL_WIDTH; tCol ++) {

            if (ctx->levelTiles[tCol][tRow] == 1) { // 1=solid/existing for now
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

    for (int e = 0; e < MAX_ENTITIES; e ++) {
        if (!ctx->ecs.hitbox[e]) continue;
        if (!ctx->ecs.texture[e]) continue;

        SDL_FRect* myHitbox = ctx->ecs.hitbox[e];
        
        SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, 255);
        SDL_RenderFillRectF(ctx->renderer, myHitbox);

        SDL_RenderCopy(ctx->renderer, *ctx->ecs.texture[e], NULL, &(SDL_Rect){
            (int)myHitbox->x, (int)myHitbox->y,
            (int)myHitbox->w, (int)myHitbox->h,
        });
    }

    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_RenderCopy(ctx->renderer, ctx->screen, NULL, NULL);

    SDL_RenderPresent(ctx->renderer);
}