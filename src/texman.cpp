#include "texman.h"
#include <string>

void TextureManager::set(SDL_Renderer *renderer) {
    this->renderer = renderer;
}

bool TextureManager::add(const char *file) {
    std::string completeFilePath = file;
    completeFilePath = "res/" + completeFilePath;

    SDL_Texture *newTexPtr = IMG_LoadTexture(renderer, completeFilePath.c_str());
    SDL_SetTextureScaleMode(newTexPtr, SDL_SCALEMODE_NEAREST);
    
    if (!newTexPtr) {
        SDL_Log("SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    textures[file] = newTexPtr;
    return true;
}

SDL_Texture* TextureManager::operator[](const char *file) const{
    return textures.at(file);
}