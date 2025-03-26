module;

import std;

import <SDL.h>;
import <SDL_ttf.h>;

export module sdlLoad;


export SDL_Texture* LoadTextureFromFile(SDL_Renderer* renderer, std::string a)
{
    SDL_Surface* image = SDL_LoadBMP(a.c_str()); //Freed later
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
}

export SDL_Texture* CreateTextureFromString(SDL_Renderer* renderer, TTF_Font* font, std::string a, SDL_Color color)
{
    SDL_Surface* image = TTF_RenderText_Solid(font, a.c_str(), color); //Freed later
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return texture;
}