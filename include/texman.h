#pragma once

#include <map>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class TextureManager{
private:
    std::map<const char*, SDL_Texture*> textures;
    SDL_Renderer *renderer;
public:
    SDL_Texture* operator[](const char *file) const;
    void set(SDL_Renderer *renderer); 
    bool add(const char *file); //return success
};