#ifndef _EVENT_H
#define _EVENT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"

void check_event(SDL_Event* eventptr, Game* gptr);
void key_press(SDL_Event* eventptr, Game* gptr, SDL_Scancode scancode);

#endif