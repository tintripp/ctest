#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class TextureManager{
public:
    std::vector<SDL_Texture*> textures;

    bool add(SDL_Renderer *renderer, const char *file); //return success
};