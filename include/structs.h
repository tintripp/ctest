#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "constants.h"

//entity
typedef uint16_t Entity;

//components
typedef struct {
    float up;
    float down;
} Gravity;

//player struct, stuff for ONLY players (e.g. onGround)
//most enemies will likely have their own behavioral component
typedef struct { 
    int jumpHeightMax;
    int jumpHeightMin;
    bool onGround;
    bool jumpCancelled;
} Player;

//ECS CONTEXT
typedef struct {
    SDL_FRect* hitbox[MAX_ENTITIES];
    Gravity* gravity[MAX_ENTITIES];
    SDL_FPoint* velocity[MAX_ENTITIES];
    SDL_Texture** texture[MAX_ENTITIES];
    Player* player[MAX_ENTITIES];
} ECSContext;

//global variables
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* screen;
    bool done;

    int levelTiles[LEVEL_WIDTH][LEVEL_HEIGHT];
    //Player player;
    ECSContext ecs;
} GameContext;