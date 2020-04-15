#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "types.h"

//void draw_status(SDL_Renderer* renderer, Game* gptr);
void draw_background(SDL_Renderer* renderer, Game* gptr);

void draw_walls(SDL_Renderer* renderer, 
                Game* gptr, 
                Rayhit* rayhit,
                uint32_t textures[256][256]);

void draw_status(   SDL_Renderer* renderer, 
                    SDL_Surface* surface, 
                    TTF_Font* font, 
                    Game* gptr, 
                    char* str);

#endif