#ifndef _MINIMAP_H
#define _MINIMAP_H

#include "types.h"
#include <SDL2/SDL_render.h>

void draw_minimap(SDL_Renderer* renderer, Game* gptr, Coord rays[], int num_rays);

#endif