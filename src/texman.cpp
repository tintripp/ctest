#include "texman.h"
#include <string>

bool TextureManager::add(SDL_Renderer *renderer, const char *file) {
    std::string completeFilePath = file;
    completeFilePath = "res/" + completeFilePath;

    SDL_Texture* newTexPtr = IMG_LoadTexture(renderer, completeFilePath.c_str());
    if (!newTexPtr) {
        SDL_Log("SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    textures.push_back(newTexPtr);
    return true;
}