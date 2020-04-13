#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL2/SDL_render.h>
#include "types.h"

void init_texture(
    Game* gptr,
    SDL_Color textures[256][256]);

#endif