#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "types.h"
#include "minimap.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

//void draw_status(SDL_Renderer* renderer, Game* gptr);
void draw_background(SDL_Renderer* renderer, Game* gptr);
void draw_walls(SDL_Renderer* renderer, Game* gptr, Coord* rayhit);

void draw_status(   SDL_Renderer* renderer, 
                    SDL_Surface* surface, 
                    SDL_Texture* texture, 
                    TTF_Font* font, 
                    Game* gptr, 
                    char* str);

#endif