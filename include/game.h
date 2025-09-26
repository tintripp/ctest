#pragma once
#include "structs.h"

void Game_Init(GameContext* ctx);
void Game_HandleEvents(GameContext* ctx);
void Game_Update(GameContext* ctx, float deltaTime);
void Game_Render(GameContext* ctx);