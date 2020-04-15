#ifndef _IO_H
#define _IO_H

#include "types.h"
#include <stdio.h>
#include <SDL2/SDL_render.h>

void print_game_status(Game* gptr, char* str);
void read_ppm_to_textures(FILE* ppm, uint32_t textures[256][256]);

#endif