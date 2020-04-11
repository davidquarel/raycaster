#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "types.h"
#include "minimap.h"
#include <SDL2/SDL_render.h>

//void draw_status(SDL_Renderer* renderer, Game* gptr);
void draw_background(SDL_Renderer* renderer, Game* gptr);
void draw_walls(SDL_Renderer* renderer, Game* gptr, Coord* rayhit);

#endif