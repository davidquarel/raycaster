#ifndef MINIMAP_HEADER
#define MINIMAP_HEADER

#include "types.h"
#include <SDL2/SDL_render.h>

void draw_minimap(SDL_Renderer* renderer, Player* me, Map* map, Coord rays[], int num_rays);

#endif